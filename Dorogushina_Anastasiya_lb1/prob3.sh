#!/bin/bash

# файл вывода, введенный файл
if [ $# -eq 0 ]; then
    echo "Файл не указан"
    exit 1
fi
target="$1"
output="prob3.txt"
> "$output"
full_path=$(realpath $target)

# поиск ссылок
i=0
find / -type l -lname "*$target" -ls 2>/dev/null | grep "$full_path$" | iconv -f UTF-8 -t UTF-8//IGNORE |
while IFS= read -r line; do
    ((i++))
    echo "$i. $line" >> "$output"
done
lines_count=$(wc -l < "$output")
if [ $lines_count -le 1 ]; then
    echo "У введенного файла нет полноименных символьных ссылок" >> "$output"
fi

# дата и время
echo "" >> "$output"
echo "Результаты в $output, дата и время: $(date)" >> "$output"