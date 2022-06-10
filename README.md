# Boolean Logic calculator and Proposition Generator

## Привет!
![Ex1](https://i.ibb.co/nkYfbMF/lt1.jpg)
![Ex2](https://i.ibb.co/J5M3GPN/lt2.jpg)

Это мой мобильный калькулятор для логики высказываний. 
Я решил написать его для того, чтобы прямо из кармана доставать студентам примеры.

### Приложение умеет: 
- решать формулы логики высказываний через построение таблиц истинности
- генерировать случайную ППФ с двумя-тремя переменными и сравнительно небольшим количеством подформул*

Генерируемые формулы можно усложнить или упростить через добавление аргументов в коде. Позже подумаю, как это можно было контролировать из интерфейса.

## Logical connectives dictionary:
| In-app character  | Connective |
| ------------- | ------------- |
| `~`  | (not) negation  |
| `->`  | (if... then) implication |
| `\|`  | (or) disjunction  |
| `&`  | (and) conjunction  |
| `=`  | (if and only if) biconditional  |
| `!`  | strong disjunction  |

## Технические комментарии
Приложение состоит из двух частей: клиентская часть написана на Java, а все вычисления и генерацию берет на себя С++, который вызывается через JNI.

## Больше примеров!
![Ex3](https://i.ibb.co/NNGNhhx/lt3.jpg")
![Ex4](https://i.ibb.co/vYYTzQM/lt4.jpg)
![Ex5](https://i.ibb.co/hRmNg9d/lt5.jpg" alt="lt5)


## More about logic
https://www.logicinaction.org/
