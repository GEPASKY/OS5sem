#! /usr/bin/bash


if [ -z "$1" ]; then
    echo "Введите путь к каталогу:"
    read DIRECTORY
else
    DIRECTORY=$1
fi

if [ -z "$2" ]; then
    echo "Введите имя выходного файла:"
    read OUTPUT_FILE
else
    OUTPUT_FILE=$2
fi


if [ ! -d "$DIRECTORY" ]; then
    echo "Ошибка: Каталог '$DIRECTORY' не существует."
    exit 1
fi


if [ -f "$OUTPUT_FILE" ] && [ ! -w "$OUTPUT_FILE" ]; then
    echo "Ошибка: Файл '$OUTPUT_FILE' существует, но недоступен для записи."
    exit 1
fi


find "$DIRECTORY" -type f -name "*.txt" -printf "%f\n" | sort > "$OUTPUT_FILE"


if [ $? -eq 0 ]; then
    echo "Список текстовых файлов успешно записан в '$OUTPUT_FILE'."
else
    echo "Ошибка при обработке файлов."
    exit 1
fi
