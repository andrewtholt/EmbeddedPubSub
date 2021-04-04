
|Offset|width|Description|
|--|--|--|
0|1|Packet Length|
1|1|Command|
2|1|Sender|
3|1|Key Length|
4|x|Key string|
x|1|Data length|
x|1|Data Type|

e.g. SET LED 1 (Boolean)

|Offset|width|Content|Description|
|--|--|--|--|
0|1|9|Packet Length, <br>not including this.|
1|1|2|Command|
2|1|1|Sender|
3|1|3|Key Length|
4|3|LED|Key string|
7|1|2|Data length|
8|1|1|Data Type|
9|1|1|Data

e.g GET LED (Boolean)

|Offset|width|Content|Description|
|--|--|--|--|
0|1|7|Packet Length,<br>not including this|
1|1|1|Command|
2|1|2|Sender|
3|1|3|Key Length|
3|3|LED|Key string|
7|1|1|Data Type|
