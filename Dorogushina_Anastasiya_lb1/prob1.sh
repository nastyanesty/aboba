#!/bin/bash

# файл вывода
if [ $# -eq 0 ]; then
    echo "Файл для вывода не указан"
    exit 1
fi
output=$1
> "$output"

# функция для поиска
search_file() {
    local type=$1
    local desc=$2
    
    echo "$desc:" >> "$output"
    ls -lR / 2>/dev/null | grep -E "^(/|$type)" | while IFS= read -r line; do
        if [ "${line:0:1}" == "/" ]; then
            current_path="${line%:}"
            [ "$current_path" == "/" ] && current_path=""
        else
            file_name=$(awk '{print $NF}' <<< "$line")
            file_data=$(awk -v dir="$current_path" -v file="$file_name" '{
                sub(file, dir "/" file); 
                print
            }' <<< "$line") 
            echo "$file_data" >> "$output"
            exit
        fi
    done
}

# исполнение поиска
search_file '^-' "Обычный файл"
search_file '^b' "Специальный файл блочного устройства"
search_file '^c' "Файл символьного устройства"
search_file '^d' "Каталог"
search_file '^l' "Символьная ссылка"
search_file '^p' "FIFO"
search_file '^s' "Сокет"

# дата и время
echo "" >> "$output"
echo "Результаты в $output, дата и время: $(date)" >> "$output"