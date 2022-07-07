# mtfind
Написать программу mtfind, производящую поиск подстроки в текстовом файле
по маске с использованием многопоточности.



Маска - это строка, где "?" обозначает любой символ.



Программа принимает в качестве параметров командной строки:

1) Имя текстового файла, в котором должен идти поиск (размер файла - до 1Гб).

2) Маску для поиска, в кавычках. Максимальная длина маски 100 символов.



Вывод программы должен быть в следующем формате:

- На первой строке - количество найденных вхождений.

- Далее информация о каждом вхождении, каждое на отдельной строке, через пробел:

номер строки, позиция в строке, само найденное вхождение.



Дополнения:

- В текстовом файле кодировка только 7-bit ASCII
- Поиск с учетом регистра

- Каждое вхождение может быть только на одной строке. Маска не может содержать символа перевода строки

- Найденные вхождения не должны пересекаться.

- Пробелы и разделители участвуют в поиске наравне с другими символами.

- Можно использовать STL, Boost, возможности С++1x.

- Многопоточность нужно использовать обязательно. Однопоточные решения засчитываться не будут.

- Серьезным плюсом будет разделение работы между потоками равномерно вне зависимости от количества строк во входном файле.



ПРИМЕР

Файл input.txt:
```
1  I've paid my dues

2  Time after time.

3  I've done my sentence

4  But committed no crime.

5  And bad mistakes ?

6  I've made a few.

7  I've had my share of sand kicked in my face

8  But I've come through.
```


Запуск программы: 

```
mtfind input.txt "?ad"
```
Ожидаемый результат:
```
3

5 5 bad

6 6 mad

7 6 had
```


Решение представить в виде архива с исходным кодом и
проектом CMake
или Visual Studio (либо в виде ссылки на онлайн Git-репозиторий).

Код должен компилироваться в GCC или MSVC.




Критерии оценки решения:
1) Правильность выдаваемых результатов

2) Качество и читабельность кода, легкость дальнейшего развития и поддержки

3) Скорость работы и потребление памяти

# Build
```
mkdir build-release
cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

# Run
```
./mtfind <input file> <mask>
```