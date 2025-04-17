#!/bin/bash

# новая директория
TEST_DIR="test_dir"
mkdir -p "$TEST_DIR"
echo "Создан каталог: $TEST_DIR"
echo -n "Начальный размер: "
du -sh "$TEST_DIR" | cut -f1

# добавление файлов
for i in {1..10}; do
    echo "meow" > "$TEST_DIR/file_$i"
done
echo -n "Размер после создания 10 файлов: "
du -sh "$TEST_DIR" | cut -f1

# добавление поддиректорий
for i in {1..10}; do
    mkdir -p "$TEST_DIR/subdir_$i"
done
echo -n "Размер после создания 10 поддиректорий: "
du -sh "$TEST_DIR" | cut -f1

# удаление файлов
rm -f "$TEST_DIR"/*
echo -n "Размер после удаления файлов: "
du -sh "$TEST_DIR" | cut -f1

# удаление поддиректорий
rm -rf "$TEST_DIR"/subdir_*
echo -n "Размер после удаления поддиректорий: "
du -sh "$TEST_DIR" | cut -f1

# синх
sync
echo -n "Размер после sync: "
du -sh "$TEST_DIR" | cut -f1

# удаление директории
rm -rf "$TEST_DIR"
echo "Каталог удален"

# дата и время
echo ""
echo "Дата и время: $(date)"