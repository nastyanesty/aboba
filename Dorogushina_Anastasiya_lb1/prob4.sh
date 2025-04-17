#!/bin/bash

# файл вывода, введенный файл
if [ $# -eq 0 ]; then
    echo "Файл не указан"
    exit 1
fi
target="$1"
output="prob4.txt"
> "$output"

# поиск ссылок
i=0
ls -lR / 2>/dev/null | grep -E "^l.*$target$|^/" |
while IFS= read -r line; do
    if [ "${line:0:1}" == "/" ]; then
        current_path="${line%:}"
        [ "$current_path" == "/" ] && current_path=""
    else
        name=$(awk '{print $(NF-2)}' <<< "$line")
        ((i++))
        echo "$i) $current_path/$name" >> "$output"
    fi
done
lines_count=$(wc -l < "$output")
if [ $lines_count -le 1 ]; then
    echo "У введенного файла нет символьных ссылок" >> "$output"
fi

# дата и время
echo "" >> "$output"
echo "Результаты в $output, дата и время: $(date)" >> "$output"