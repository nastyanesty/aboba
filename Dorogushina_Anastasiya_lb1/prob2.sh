#!/bin/bash

# файл вывода, введенный файл
if [ $# -eq 0 ]; then
    echo "Файл не указан"
    exit 1
fi
target="$1"
output="prob2.txt"
> "$output"
echo "Введенный файл: $(realpath "$target")" >> "$output"

# проверка на то, что нет ссылок
count=$(( $(ls -liR "$target" | cut -d' ' -f3) - 1 ))
if [ $count -eq 0 ]; then
    echo "У введенного файла нет жестких ссылок" >> "$output"
else
    inode=$(ls -liR "$target" | cut -d' ' -f1)
    echo "Inode: $inode, жёстких ссылок: $count" >> "$output"
    # поиск файлов с таким же inode
    i=0
    ls -liR / 2>/dev/null | grep -E "^$inode|^/" |
    while IFS= read -r line; do
        if [[ "${line:0:1}" == "/" ]]; then
            path="${line%/*}"
            continue
        fi
        name=$(awk '{ print $NF }' <<< "$line")
        if [[ "$name" != "$target" && ! -z "$path" ]]; then
            i=$((i + 1))
            echo "$i. $path/$name" >> "$output"
            
            if [[ $i -ge $count ]]; then
                break
            fi
        fi
    done
fi

# дата и время
echo "" >> "$output"
echo "Результаты в $output, дата и время: $(date)" >> "$output"