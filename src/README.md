
|Offset|width|Description|
|--|--|--|
0|1|Packet Length|
1|1|Command|
2|1|Key Length|
3|x|Key string|
x|1|Data length|
x|1|Data Type|

e.g. SET LED 1 (Boolean)

|Offset|width|Content|Description|
|--|--|--|--|
0|1|8|Packet Length|
1|1|2|Command|
2|1|3|Key Length|
3|3|LED|Key string|
6|1|2|Data length|
7|1|1|Data Type|
8|1|1|Data

e.g GET LED (Boolean)

|Offset|width|Content|Description|
|--|--|--|--|
0|1|6|Packet Length|
1|1|1|Command|
2|1|3|Key Length|
3|3|LED|Key string|
6|1|1|Data length|
7|1|1|Data Type|
