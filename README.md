# Кулькулятор для логики высказываний с генератором выражений

| Example 1  | Example 2 |
| ------- | ------ |
| <img src="https://i.ibb.co/nkYfbMF/lt1.jpg" width="330">  | <img src="https://i.ibb.co/J5M3GPN/lt2.jpg" width="330"> | 

Это мой мобильный калькулятор для булевой алгебры / логики высказываний. 
Я решил написать его для того, чтобы прямо из кармана доставать студентам примеры, например, во время зачетов.

### Приложение умеет: 
- решать формулы логики высказываний через построение таблиц истинности (формула вбивается в верхнее поле, решение происходит после нажатаия 'Solve').
- генерировать случайную ППФ с двумя-тремя переменными и сравнительно небольшим количеством подформул (после нажатия кнпоки 'Random!' сгенерированная формула появится в верхнем поле).

### Решение
Если бы студент строил таблицу истинности для формулы ```p -> q```, то у него бы получилось следующее:
| p  | q | p -> q |
| ------- | ------ | ------ |
| F | F | T | 
| F | T | T |  
| T | F | F | 
| T | T | T | 

Приложение вместо этого выведет список строк в таблице истинности в формате:

|  p -> q |
| -------- |
| p = F, q = F ==> TRUE | 
| p = F, q = T ==> TRUE | 
| p = T, q = F ==> FALSE | 
| p = T, q = T ==> TRUE | 

С помощью этой информации пользователь может понять, является ли формула выполнимой и является ли она тавтологией.

## Словарь связок | Logical connectives dictionary:
Приложение не использует привычные логические символы: ```¬```, ```∨```, ```→``` и т.д. Вместо этого пользователю нужно использовать следующие символы:

| In-app character  | Connective |
| ------------- | ------------- |
| `~`  | (not) negation  |
| `->`  | (if... then) implication |
| `\|`  | (or) weak disjunction  |
| `&`  | (and) conjunction  |
| `=`  | (if and only if) biconditional  |
| `!`  | strong disjunction  |

Для упрощения ввода в интерфейсе предусмотрены кнопки для быстрого ввода специальных символов.

## Технические комментарии
Приложение состоит из двух частей: клиентская часть написана на Java, а все вычисления и генерацию берет на себя С++, который вызывается через JNI.

## Больше примеров!

| Case 1  | Case 2 | Case 3|
| ------- | ------ | ----- |
| <img src="https://i.ibb.co/NNGNhhx/lt3.jpg" width="330">  | <img src="https://i.ibb.co/vYYTzQM/lt4.jpg" width="330"> | <img src="https://i.ibb.co/hRmNg9d/lt5.jpg" width="330"> |


## More about logic
https://www.logicinaction.org/
