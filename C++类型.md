# 数据类型

## 默认类型

- 基本类型  -- 整数和浮点数 (第三章 处理数据)

- 复合类型 -- 数组 字符串 指针 结构(第四章 复合类型)

### 基本类型

#### 整型

`char`   `short`   `int`   `long`   `long long`

##### 大小

1字节 = 8bit

| 类型        | 字节数 | C格式化输出 | 范围                             |
| ----------- | ------ | ----------- | -------------------------------- |
| char        | 1      | %c          | -2^7^~-2^7^-1  或   0~-2^8^-1    |
| int         | 4      | %d          | -2^31^~-2^31^-1  或   0~-2^32^-1 |
| short       | 2      | %d          | -2^15^~-2^15^-1                  |
| long        | 8      | %ld         | 0~-2^64^ -1                      |
| float       | 4      | %f          | 0~-2^32^-1                       |
| double      | 8      | %lf         | 0~-2^64^-1                       |
| long double | 16     |             | 0~-2^128^-1                      |
| wchar_t     | 2 或 4 | %c          | 0~-2^16^-1 或 0~-2^32^-1         |

##### char

专为存储字符而设计。

C++对字符用单引号`'M'`

cin 和 cout 会根据类型进行输出

```cpp
char ch;
cin >> ch;
cout << ch;
```

输入 `77` 或 `M` 最终的输出都是 `M`

Unicode中，未来能够存储更多的值，引入`wchar_t` 

找出编码 88 对应的字符

```cpp
// 方法1
char ch = 88 ; 
cout << ch << endl;
// 方法2
cout.put(char(88));
cout << char (88) << endl;
cout << (char) 88 << endl;
```



##### 其他

###### 整型溢出

`signed int` 类型的变量设置为 2147483647(2^31^-1)， 加1则溢出 `-2147483648`

`unsigned int` 类型的变量设置为 4294967295(2^32^-1)， 加1则溢出 `0`

###### 整型字面值

整型字面值是显式地书写的常量。

三种计数方式 

基数为10 `10`  基数为8 `07`   基数为16 `0x6a`    `0X6A` 

iostream提供控制符dec hex oct分别用于指示cout以十进制(默认)，十六进制，八进制格式显式整数。

```cpp
int myNum = 16;

cout << dec; // 十进制 默认
cout << "dec : " << myNum << endl;
cout << hex; // 十六进制
cout << "hex : " << myNum << endl; 
cout << oct; // 八进制
cout << "oct : " << myNum << endl;

/*
dec : 16
hex : 10
oct : 20
*/
```

###### 整数除法

两个操作数都是整数，则C++执行整数除法。结果的小数部分被丢弃，得到的结果是一个整数。如果其中一个(或两个)是浮点数，则小数部分保留，结果为浮点数。



#### 浮点型

`float` 32位 `double` 64位

`long double` 80，96或128位

可以表示整数之间的值，有缩放因子可表示的范围大得多

运算速度慢，精度降低。

##### 书写格式

```cpp
float  myNum = 12.34;
double myNum = 1.234E2;
double myNum = 1.234E+2;
double myNum = 1.234E-2;
```

float 需要 f 或 F后缀

```c
12.3f
1.23E1F
```

double 默认

```c
1.23E1
```

long 需要 l 或 L后缀

```c
2.2L // 常用 L更易区分
2.2l
```





##### cout

通常cout会删除结尾的零

`3333333.250000` 显示为 `3333333.25`

调用`cout.self()`可以覆盖这种行为。

```cpp
cout.self(ios_base::fixed,ios_base::floatfield); //fixed-point
float myFloat = 10.0/3.0;
cout << myFloat;
```



#### bool

布尔变量的值可以是true或false。

- 非零值解释为true，

- 零`0` 或 空字符`\0`解释为false。

```cpp
bool myTrue;
myTrue = 1;
myTrue = true;
myTrue = -100;
```

```cpp
bool myFalse;
myFalse = 0;
myFalse = false;
```





### 复合类型

#### 数组array

**数组array** 存储多个同类型的值

- 存储在每个元素中的值的类型
- 数组名
- 数组中的元素数

##### 元素访问

```cpp
// 单独访问数组元素
months[0] = 10;  // 数组必须从0开始编号
months[11]; // 最后一个元素的索引比数组长度小1
// 编译器不会检查下标是否有效
/* 编译器并不会指出错误,但赋值可能引发问题 */
/* 它可能破坏数据或代码,也可能导致程序异常终止 */
months[100] = 10; 
```

##### 初始化

```cpp
int seasons[4];
int seasons[4] = {1,2,3,4}; // 全部初始化
int seasins[]  = {1,2,3,4}; // 方括号为空，则C++编译器将自动计算元素个数。
int seasons[4] {1,2,3,4};   // 可忽略等号
// 只对数组的一部分进行初始化
int seasons[4] = {1,2};
/* 只对数组的一部分进行初始化，则编译器将其他元素设置为0
  所以,要将数组中所有的元素都初始化为0，
  显式地将第一个元素初始化为0即可 */
// 将所有元素都设置为零
short days[100] = {};
short days[100] = {0};
```

##### 禁止缩窄转换

```cpp
// 禁止缩窄转换
long days[] = {1,2,3.0};      // not allowed
char days[] = {'1',122222222};// not allowed
char days[2] = {'1',112,'\0'};    // ok
```

##### 数组名 数组地址

两者从数字上相同，但类型不同。

`int pa[5] = {1,2,3,4,5};`

- `pa`指向第一个元素的指针也就是`&pa[0]` 
- `&pa`整个数组的地址。

差别体现在，

- 加法运算时，二者跳过的字节数不同。
- 要得到第一个元素的值，`pa`需要解除一次引用，`&pa`需要解除两次引用

`**&pa == pa* ==pa[0]`

##### sizeof

将 sizeof运算符 用于数组名，得到的是整个数组中的字节数，

将sizeof用于数组元素，则得到的将是元素的长度(单位，字节)。

数组元素的个数。sizeof(数组名)/sizeof(数组名[0])

```cpp
int myArray[3] = {1,2,3};

cout << "整个数组的字节数" 
    << sizeof(myArray) << endl;
cout << "单个数组元素的字节数" 
    << sizeof(myArray[0]) << endl;
cout << "数组中元素的个数" 
    << sizeof(myArray)/sizeof(myArray[0]) << endl;
```

##### 二维数组

C++没有提供二维数组类型，但用户可以创建每个元素本身都是数组的数组。

```cpp
/* 包含4个元素的数组，每个元素都是一个由5个整数组成的数组 */
int my2dArr[4][5]; 
// 初始化
int my2dArr[4][5] = 
{
    {1,2,3,4,5}, // my2dArr[0]
    {1,2,3,4,5}, // my2dArr[1]
    {1,2,3,4,5}, // my2dArr[2]
    {1,2,3,4,5}, // my2dArr[3]
}
```

###### 字符串

从存储空间的角度说，指针数组更为经济，然而要修改字符串则二维数组是更好的选择

###### 指针数组

```cpp
const char * cities[5] = // an array of pointers to string
{
	"A City",
	"B City",
	"C City",
	"D City",
	"E City"
};
cout << cities[1]; // 输出的内容是 字符串 B City
```

###### char数组的数组

```cpp
char cities[5][25] =
{
	"A City",
	"B City",
	"C City",
	"D City",
	"E City"
};
cout << cities[1]; // 输出的内容是 字符串 B City
```

###### string对象数组

```cpp
const string cities[5] =
{
	"A City",
	"B City",
	"C City",
	"D City",
	"E City"
};
cout << cities[1]; // 输出的内容是 字符串 B City
```



#### 字符串

将字符串读入程序时，应使用已分配的内存地址。

- 可以是数组名
- 可以是new初始化过的指针

##### C风格字符串

使用字符常量`'a'`

以空字符 `\0` 结尾，其ASCII码为0。标记字符串的结尾。  空字符`\0`转换为bool值，为false。因此可以配合C-字符串，使用循环`while(arr[i]){i++}`

```cpp
// 虽然dog1 dog2都是char数组，但只有dog2是字符串
// 不是字符串
char dog1[3] = {'d','o','g'};
// 字符串
char dog2[4] = {'d','o','g','\0'};
```

很多C++中处理字符串的函数，例如`cout`，会逐个处理字符串中的字符，直到到达空字符为止。

- cout 打印dog1的3个字母后，会接着将内存中随后的各个字节解释为要打印的字符，**直到遇到空字符为止**。(空字符在内存中很常见，所有有时并不容易发现错误)
- cout 可以准确打印dog2中的前3个字母

###### 字符串常量

字符串常量 string constant  字符串字面值 string literal

使用字符串常量`"S"`初始化字符数组，不同于字符常量`'S'`，它表示的是一个内存地址，内容是`字符S 和 \0 组成的字符串`。

> 字符串常量 `"string constant"`  被判定为字符串开始的地址。
>
> cout对象将char地址解释为打印字符串。`cout << "string constant";`
>
> 类型转换(int *) 将地址转换为int指针后，将被作为地址打印。`cout << (int*)"string constant";`

应确保数组足够大，能够存储字符串中所有字符(包括空字符)。

各种C++输入工具通过键盘输入，将字符串读入到char数组中时，会自动加上结尾的空字符。

可以使用`\0`截短字符串

赋值过程是将字符串常量`"Bird"`的首地址进行传递，数组名`bird1`也是一个char的地址

有些编译器将字符串常量视为**只读常量**，不可以修改。

> `char charr[10] = "ok";`显示字符串的地址不能使用charr 

有些编译器只使用字符串字面值的一个副本来表示程序中所有的该字面值。

> 将值读入一个字符串，可能影响 位于其他地方的字符串。
>
> 不能尝试将信息读入未初始化的指针，因为并不知道信息将被存储在哪里(甚至有可能改写内存中的信息)。可以通过使用足够大的char数组来接受输入，或使用std::string对象

```cpp
// 隐式地包括结尾的空字符 \0
char bird1[11] = "Bird";
char bird2[] = "Bird"; 
cout << "bird" << endl;
// bird1 bird2表示的是一个内存地址，内容是字符B i r d \0 组成的字符串。
```

###### 字符串字面值

除char类型外，`wchar_t`  `char16_t`  `char32_t` 都有对应的字符串字面值。

前缀分别是  `L`  `u`   `U`

```cpp
wchar_t  charr1[] = L"My wchar_t array";
char16_t charr2[] = u"My char16_t array";
char32_t charr3[] = U"My char32_t array";
```

Unicode字符编码方案 `UTF-8` 前缀`u8`，根据编码的数字值，字符可能存储1~4个八位组。

原始字符串 raw，前缀`R`。

> 原始字符串中，字符表示的就是它自己，不存在换行符之类的玩意。
>
> `\n`就只是 \和n两个字符而已。
>
> 回车键不会移到下一行而是在原始字符串中添加回车字符。

```cpp
cout << R"( "\n" is \ and n instead of endl)" << endl;
// 使用标准字符串字面值
cout << " \"\\n\" is \\ and n instead of endl" << endl;
```

可以修改原始字符串起始条件以满足更多的输入需求。

> `"(`  和 `)"` 可以替换成 `"除 空格 ( ) \ 控制字符之外的任意字符(`  和 `)..."`

```cpp
cout << R"+—( "\n" is \ and n instead of endl)+-" << endl;
```

可以配合其他字符串前缀结合使用`Ru`  `UR`等





##### 拼接

C++允许拼接字符串字面值，将两个用引号括起的字符串合并为一个。

(不考虑`\0`，第一个字符串的`\0`被第二个字符串的第一个字符取代)

不会在被连接的字符串之间添加空格。

```cpp
cout << "string1" "string2 \n"; 
```

##### 保存数组

`cstring`   老式实现`string.h`

字符串中元素的个数 字符串的长度 `strlen(数组名)`(单位是不含\0的字符数) 

> 函数strlen 返回第一个空字符的位置，所以对于未初始化的数据，大小是"随机"的 

char数组每个元素占1字节，所以下面两项相等。

数组本身的长度 `sizeof(数组名) `单位是字节。

数组元素的个数  `sizeof(数组名)/sizeof(数组名[0])  `



```cpp
#include <cstring>
...
const int Size = 15;
// 实现1
char name1[Size];
cin >> name1;
// 实现2
char name2[Size] = "C++ string";
cout << name1 
    << "字符串的长度" << strlen(name1)   // 10 
    << "数组本身的长度" << sizeof(name1)/sizeof(name1[0])// 15
    << endl; 
```

##### 拷贝

`#include <cstring>`

strcpy(目标字符串，拷贝内容);

```cpp
char charr1[10];
strcpy(charr1, "ok");
cout << charr1 << endl;
```

strncpy(目标字符串，拷贝内容,最大拷贝字符数);

> 如果在到达字符串结尾之前，目标内存已经用完，则不会添加空字符，需要手动tian'jia

```cpp
const int ArrSize = 10;
char charr2[ArrSize];
strncpy(charr2, "okkkkkkkkkkkkkkkkkk",ArrSize);
charr2[ArrSize] = '\0' 

```



##### 输入

###### cin每次读取一个单词

cin将使程序跳过空白，直到找到非空白字符为止。然后读取字符。直到再次遇到空白为止。

cin使用空白来确定字符串的结束位置

> 空白(包括 空格 ` ` 制表符`\t` 换行符`\n`)

这意味着cin获取字符串输入时只读取一个单词(空白会导致读取结束)，读取单词后将字符串放入数组，在结尾自动添加空字符`\0`。

```cpp
const int ArrSize = 16;
char name[ArrSize];
char dessert[ArrSize];

cout << "Enter ur name:";
cin >> name;
cout << "\nEnter ur dessert:";
cin >> dessert;
cout << endl;
cout << name << ":" << dessert;
```

理想的输出

```cpp
Enter ur name:Zhang
Enter ur dessert:cola
zhang:cola
```

但是一旦输入不是一个单词就会出现

```cpp
Enter ur name:Zhang Pengyi
Enter ur dessert:
zhang:Pengyi
```

###### 读取一行输入

istream提供 `getline()`  `get()` 读取一行字符串输入，直到到达换行符。

`getline()` 丢弃换行符。

`get()` 将换行符保留在输入序列中

> 推荐使用`cin.get`函数
>
> - 老式实现没有getline()
> - get()使输入更仔细，
>
> > 例如查看下一个输入字符，如果是换行符说明已经读取了整行。
> >
> > 否则，该行中还有其他输入

`cin.getline()`  

丢弃换行符。用空字符`\0` 替换换行符。

输入的换行符：通过**回车键**来确定输入结尾。

参数：

- 第一个参数 存储输入行的数组的名称
- 第二个参数 读取的字符数(最多读取的字符比该参数少1，余下空间用于存储`\0`)

```cpp
const int ArrSize = 48;
char name1[ArrSize];
char name2[ArrSize];

cin.getline(name1,ArrSize); // 这行包括的字符不超过47个
// 连续的两行分别读入 name1 name2
cin.getline(name1,ArrSize).getline(name2,ArrSize);
```



`get()` 

两种形式`cin.get(存储输入行的数组的名称,读取的字符数);`  `cin.get()`

有参调用后，换行符`Enter`将留在输入队列中，第二次调用时看到的第一个字符就是换行符。get()认为已到达行尾，没有发现可读取的内容。

因此需要调用`cin.get();` “跨过”该换行符。

```cpp
const int ArrSize = 48;
char name1[ArrSize];
char name2[ArrSize];
// 方法1
cin.get(name1,ArrSize); // 读取第一行
cin.get(); // 允许阅读新的一行
cin.get(name2,ArrSize); // 读取第二行
// 方法2
// 合并两个类成员函数 允许阅读新的一行
cin.get(name1,ArrSize).get(); // 读取第一行 并且允许读取第二行
cin.get(name2,ArrSize); // 读取第二行
```

###### 空行

`cin.getline()` 或 `cin.get()` 读取空行。

最初，下一条输入语句将在前一条`cin.getline()` 或 `cin.get()` 结束读取的位置，继续读取。

当前，`get()`读取空行后，将设置失效位(failbit)。接下来的输入将被阻断。

需要通过`cin.clear();`命令恢复输入。

```cpp
char name1[ArrSize];
char name2[ArrSize];
cout << "输入名字1\n";
cin.get(name1, ArrSize).get();
cin.clear(); // 避免空行引发错误 但是实测不好用XD
cout << "\n输入名字2\n";
cin.get(name2, ArrSize).get();
cin.clear();
```

###### 混合输入 字符串 数字

cin读取数字将回车键生成的换行符留在输入队列中。

后面的`cin.get(name1, ArrSize)` 看到换行符，认为这是空行，将空字符串付给address数组。

解决方法：下一条输入前，读取并丢弃换行符。

> `cin.get();` 或 `char ch; cin.get(ch)`ch保存换行符。
>
> 也可以将调用进行拼接 `(cin >> num).get()`

```cpp
const int ArrSize;
char name1[ArrSize];
int num;

cout << "输入数字\n";
cin >> num;
cin.get(); // 读取并丢弃换行符
cout << "\n输入名字\n";
cin.get(name1, ArrSize).get();
cin.clear();

cout << "name1:         " << name1
	<< "     " << sizeof(name1) << endl;
cout << "num:           " << num << endl;
```





#### 结构体 struct

C++允许声明结构变量时，省略关键字struct。

结构标记的用法与基本类型名相同，这意味着，结构声明定义了一种**新类型**。

```cpp
const int NameSize = 48;
// 声明
struct myStruct
{
    char name1[48];
    string name2;
    float weight;
    double price;
};

// 定义结构体变量
myStruct frits = {}; 
/* 大括号为空
  各个成员都将被设置为零，
  字符串的每个字节都被设置为零，浮点型 整形数据设置为零*/
myStruct frits = {"my frits","my frits",0.3,2.5};
myStruct frits {"my frits","my frits",0.3,2.5};
myStruct burger = 
{
   "my burger",
   "my frits",
    0.93,
    9.9
};

// 使用
cout << burger.name << burger.price;
cout << burger.name[0];
```

结构声明 存在 **内部声明** 和 **外部声明** 之分。类似全局变量，局部变量。

C++不提倡外部变量，但**提倡使用外部结构声明**。

##### 结构数组

```cpp
MyStruct ArrStruc[3];
ArrStruc = {
    {"my frits","my frits",0.3,2.5},
    {"my frits","my frits",0.3,2.5},
    {"my frits","my frits",0.3,2.5}
};

ArrStruc[0] = {"my frits","my frits",0.3,2.5};

cout << ArrStruc[0].price << endl;
```



##### 位字段

有些信息在存储时，并不需要占用一个完整的字节， 而只需占几个或一个二进制位。为了节省存储空间，并使处理简便，C语言又提供了一种数据结构，称为“位域”或“位段”。

- 位域

  - 把一个字节中的二进位划分为几个不同的区域， 并说明每个区域的位数。

  - 每个域有一个域名，允许在程序中按域名进行操作。 这样就可以把几个不同的对象用一个字节的二进制位域来表示。

- 位字段
  - 每个成员都被称为位字段。

```cpp
enum {red;}

struct MyRegister
{
    // 位字段:指定占用特定位数的结构成员。
    // - 字段类型 是 整型 或 枚举，指定了使用的位数。
    unsigned int SN1 : 4;
    unsigned int SN2 : red;
    // - 可以使用没有名称的字段来提供间距，它不需要初始化。
    unsigned int : 4;
    bool ok :1;
}

MyRegister tr = {15, 1, true};

// Implicit truncation from 'int' to bit-field changes value from 16 to 0
// 只保留 低x位
MyRegister tr_err = {16, 1, true}; // error
```





#### 共同体union

共用体是一种数据格式，能够存储 `int`  `long`   `double` 等类型，但只能**同时**存储其中的**一种类型**。

可以使用共用体的对象，存储 `int`  `long` 或  `double`，但要求不再同一时间下进行。

共用体的长度位其最大成员的长度。当数据项不同时的使用多种格式时，会节省空间。

###### 定义

```cpp
union MyUnion
{
    int int_val;
    long long_val;
    double double_val1;
    double double_val2;
};

MyUnion union1;
union1.int_val = 1;
cout << union1.int_val << endl;

union1.long_val = 1;
cout << union1.long_val << endl;

union1.double_val1 = 1.22;
cout << union1.double_val1 << endl;
```

###### 与结构体配合

```cpp
struct widget{
    int type;
    union id
    {
        long id_num;
        char id_char[20];
    }id_val;
};
...
widget prize;

if(prize.id == 1)
    cin >> prize.id_val.id_num;
else
    cin >> prize.id_val.id_char;
```

匿名共用体

匿名共用体中，共用体的成员 会被视为 结构成员(尽管他们地址相同)

```cpp
struct MyStruct
{
    int type;
    union {
        int id_num;
        char id_char[48];
    };
}

MyStruct str1;
if(str1.type == 1)
    cin >> str1.id_num;
else
    cin >> str1.id_char;
```



#### 枚举 enum

enum 可以定义新类型，代替const，用于创建符号常量。

```cpp
enum spectrum{red,orange = 3,yellow};
```

默认情况，将整数值赋给枚举量，0,1,2,3....。

`orange = 3`可以显式的覆盖默认值，后续值同样的逐个加一。



##### 声明

在类中声明一个枚举，

- 使用枚举只是为了创建符号常量，并不会创建类数据成员，因此不需要提供枚举名。
- Months只是一个符号名称，在编译器将用12来替换它。

> ```cpp
> class Myclass
> {
> private:
> enum {Months = 12};
> // Months只是一个符号名称，在编译器将用12来替换它。
> doule arr[Months];
> ...
> }
> ```

- 将枚举量的作用域设置为类后，使用枚举名来限定枚举量，就可以避免名称冲突。

  > 关键字`struct`可代替`class`
  >
  > ```cpp
  > /* 产生冲突 */
  > enum myenum1 {Small, Medium, Large};
  > enum myenum2 {Small, Medium, Large};
  > /* 使用枚举名 限定枚举量 */
  > enum class  myenum1 {Small, Medium, Large};
  > enum struct myenum2 {Small, Medium, Large};
  > /* 使用 */
  > myenum1 choice = myenum1::Large;
  > ```

之前版本，为了**作用域内**枚举的类型安全枚举不能隐式地转换为整型，必要时可进行显式类型转换。

```cpp
/* 作用域内 */
enum class  myenum1 {Small, Medium, Large};
int num = int(myenum1::Small);
/* 非作用域内没这个担忧 */
enum myenum2 {Small, Medium, Large};
int num = Small; // 对的
```

但C++11将枚举的底层类型设置为int，还可以通过`enum class :short myenum{Small,M,L};` 中的`:short`指定枚举的底层类型。



##### 注意类型转换

对于枚举，只定义了赋值运算符。为了尊重可以执行，不要乱来。

```cpp
spectrum band;
band = red; // 允许
band = orange;
//band = 5;   // 不允许
//band = red + orange;   // 不允许
//band++;     // 不允许
band = spectrum(3); // 允许 强制类型转换，但需要遵守枚举的q
```

但枚举量是整型，可以被转换位int类型。(int不能自动转换位枚举类型)

```cpp
int color = red; // color = 0;
color = red + 3 ; // color = 0 + 3;
```

##### sizeof

枚举变量，由枚举类型定义的变量。枚举变量的大小，即枚举类型所占内存的大小。由于枚举变量的赋值，一次只能存放枚举结构中的某个常数。所以枚举变量的大小，实质是常数所占内存空间的大小（常数为int类型，当前主流的编译器中一般是32位机器和64位机器中int型都是**4个字节**），枚举类型所占内存大小也是这样。



##### 作用域为类的常量

声明类只是描述了对象的形式，并没有创建对象。创建对象前，没有用于存储值的空间。

```cpp
/* 下述操作是不可行的，声明类只是描述了对象的形式，并没有创建对象 */
/* 创建对象前，没有用于存储值的空间 */
class Myclass
{
    private:
    const int Months = 12;
    doule arr[Months];
    ...
}
```

###### 枚举

**解决方法1：枚举**

在类中声明一个枚举，

- 使用枚举只是为了创建符号常量，并不会创建类数据成员，因此不需要提供枚举名。
- 其作用域是整个类。
- Months只是一个符号名称，在编译器将用12来替换它。

```cpp
class Myclass
{
    private:
    enum {Months = 12};
    // Months只是一个符号名称，在编译器将用12来替换它。
    doule arr[Months];
    ...
}
```

###### 静态常量static

**解决方法2：静态常量**

**关键字static**

创建静态变量Months，该常量于其他静态变量存储在 `data`静态全局区，而不是存储在对象中。

它的作用域：被所有的`Myclass`对象共享。

```cpp
class Myclass
{
    private:
    static const int Months = 12;
    // Months只是一个符号名称，在编译器将用12来替换它。
    doule arr[Months];
    ...
}
```



## class

### class与struct

##### 结构体 struct 类型加强 

c++既可以定义成员变量，也可以定义成员函数，

定义结构体变量不需要加 struct 关键字

##### **区别**

- **类**中默认的成员访问权限是 **private** 的，而 **struct** 中则是 **public** 的。
- 从**类**继承默认是 **private 继承**，而从 **struct** 继承默认是 **public 继承**。
- **类**可以**定义模板**，**使用函数**，而 struct 不可以。
- **.** 与 **->** 运算符

> **.**（点）运算符和 ->（箭头）运算符用于引用类、结构和共用体的成员: 
>
> - . 点运算符应用于实际的对象。
> - -> 箭头运算符与一个指向对象的指针一起使用。

```c++
struct Student{ 
    string mName; 
    int mAge; 
    void setName(string name){ mName = name; } 
    void showStudent(){
        cout << "Name:" << mName << " Age:" << mAge << endl; 	} 
};
// c++中定义结构体变量不需要加 struct 关键字
void test01(){ 
    Student student; 
    student.setName("John"); 
    student.showStudent(); 
}
```



##### 类中声明struct

类中可以嵌套**结构声明**或**类声明**。作用域为类内。（一些较老的编译器不支持，嵌套结构的结构和类）

- 如果声明是在类的**公有部分**进行的，则可以在类外部通过**作用域解析运算符**`::`使用被声明的类型。`Queue::Node`
- 如果声明是在类的**私有部分**进行的，则只能在类中使用。



## POD类型

### 概念

POD ，是 C++ 定义的一类数据结构概念，**能用 C 的 memcpy() 等函数进行操作的类、结构体就是 POD 类型的数据**。比如 `int`, `bool`, `float`, `char*`, `int[]`等。

POD 是 Plain Old Data 的缩写

- Plain 代表它是一个普通类型

- Old 代表它是旧的，

  - 与几十年前的 C 语言兼容：意味着可以使用 memcpy() 这种最原始的函数进行操作。

  - 两个系统进行交换数据，

    - 如果没有办法对数据进行语义检查和解释，那就只能以非常底层的数据形式进行交互，

    - 拥有 POD 特征的类或者结构体通过二进制拷贝后依然能保持数据结构不变。

### 判别

#### `is_trivial`  `is_pod`

基本上谈到这个概念，一般都是说某某 class、struct、union 是不是 POD 类型的。

- 可以用 `is_pod<T>::value` 来判断。

- 什么样的类、结构体是拥有 POD 特性的呢？要求：

  - trivial（很平凡、很普通）

    - 判断
      - 用 `std::is_trivial<T>::value` 来判断是否“平凡”

    - 要求
      - 不能有 **虚函数** 和 **虚基类**
      - 要有默认的 **构造/析构函数**、**拷贝/移动构造函数**、**拷贝/移动运算符**，
        - 以上函数不显式声明用编译器自动生成
        - 也可以，用 C++ 11 的 default 关键字。

  - 布局有序。

    - 所有的非静态数据**有相同的访问级别**
    - **只要有父类，普通成员只能在其中一个类中，不可分散**。因为 C 没有继承的概念，所以就别把普通成员在两个类中都写，写在一个类中就像 C 的风格了。

### memcpy

对POD类型使用memcpy()

```cpp
class POD_class {
public :
    int a; int b;
};

int main() {

    std::cout << typeid(POD_class).name() << " is_pod:" 
        << is_pod<POD_class>::value << std::endl;
    std::cout << typeid(POD_class).name() << " is_trivial:" 
        << is_trivial<POD_class>::value << std::endl;
    
    POD_class mPod;
    mPod.a = 1;
    mPod.b = 2;
    char* p = new char[sizeof(POD_class)];
    memcpy(p, &mPod, sizeof(POD_class));
    
    POD_class *mPod_p = reinterpret_cast<POD_class*>(p);
    std::cout << mPod_p->a << " " << mPod_p->b << std::endl;
    return 0;
}

```



# C++17 类型

## std::variant

[你需要了解有关C++17中std::variant的全部信息 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/607734474)

[C++三剑客之std::variant(二)：深入剖析-CSDN博客](https://blog.csdn.net/haokan123456789/article/details/135534042)

> 在C++17标准出现之前，有一些神奇的术语，像"discriminated union"，"type-safe union"和"sum type"。后来它们都意味着相同的类型："variant"



### 概念

#### 类型&生命周期

##### union

union 处理一些底层的逻辑，并且只使用基本类型

缺陷

- 没有办法知道当前使用的类型是什么
- 它不会主动调用underlying types的析构函数

缺陷体现在使用上：

- 需要程序员进行大量的维护存储。

  在切换到新的存储对象之前，程序员必须

  - 知道（/记住，因为没办法查询）当前使用的是那种类型

  - 调用其析构函数/构造函数。

- 不推荐，将 union 用于类型双关

- union存储类型

  - 主要还是用于基本类型
  - 不会看到“高级”类型，例如vector、string或者其他的容器等

##### 解决union缺点

解决了union的两个缺点：

- 能清楚的知道当前激活的类型

- 对复杂类型的生命周期的支持：

  - 如果切换类型，会调用对应的析构函数。
  - variant会自动调用非平凡类型的析构函数和构造函数。

体现在使用上：

- 因此可以使用复杂类型

不能保存引用、数组和void类型



#### 内存大小/性能

内存大小 和`union`类似

- 遵守对齐规则。

- 采用存储类型的最大大小，再多的一点存储空间

  - 需要保证类型安全：
    - 类型安全--不允许获取未激活类型的值
    - 例如需要知道当前variant中激活的类型
  
  - variant不会分配任何额外的堆空间（不会发生额外的堆内存分配）。
  
  以下是一些储存大小测试数据：

运行时性能：

- 虽然variant为了保证类型安全多占用了几个字节的存储空间

- 但它不会多花费运行时的性能









#### 使用场景

std::variant：其他的使用场景

- 所有可能为单个变量获得几种类型的地方：
  - 解析命令行、ini文件、语言解析器等。
  - 有效地表达计算的几种可能结果：例如求解方程的根
- 错误处理：
  - 例如，您可以返回`variant<Object, ErrorCode>`，如果返回值是有效的，则返回`Object`，否则分配一些错误码（C++23可以使用`std::expected`）。
  - 实际上 optional也是可以的
- 状态机
- 不使用虚表和继承实现的多态（visiting pattern）





### 手动管理union



```cpp
#include <variant>
#include <iostream>
#include <string>
#include <vector>

union gUnion
{
    std::string str;
    std::vector<int> vec;
    ~gUnion() { } // what to delete here?
};

int main()
{
    // 初始化 Union 为一个其中一个复杂类型
    // - 此时，将 Union 视作其它类型(例如调用 Union.vec)是未定义行为
    gUnion unionObj = {"Hello, world"};
    std::cout << "unionObj.str = " << unionObj.str << '\n';
    // - 不更改激活类型的话，倒是可以连续使用
    unionObj.str = "OK, world";
    std::cout << "unionObj.str = " << unionObj.str << '\n';

    // 现在，把 Union 更改为其它类型
    // - 调用原类型对应成员的析构函数destructor
    unionObj.str.~basic_string<char>();
    // - 调用新类型的constructor!
    new (&unionObj.vec) std::vector<int>;

    // 现在，新的类型视为被激活
    // now, s.vec is the active member of the union
    unionObj.vec = {1, 2, 3, 4};
    std::cout << " vector 大小 = " << unionObj.vec.size() << '\n';
    for (auto mInt : unionObj.vec) {
        std::cout << mInt << ' ';
    }

    // - 使用结束后，记得调用此时的析构函数
    unionObj.vec.~vector<int>();
    return 0;
}

```



### variant构建

##### monostate

默认值——不赋值时

```cpp
// Iris_param类没有默认构造函数，报错
std::variant<Iris_param, std::string> iris_variant;
// pass
std::variant<std::monostate, Iris_param, std::string> iris_variant;
```

- 使用第一个类型的`默认构造函数` 进行初始化。

  若第一个类型没有 `默认构造函数` ，初始化失败 编译器报错。

- 推荐做法：

  - 将`std::monostate`作为variant的第一个类型来使用。以防止variant中的第一个类型没有默认构造函数
  - `std::monostate`只是一个空类型，被用来和variant一起使用来表示空状态。



##### 推导类型

```cpp
// 自动推导类型
std::variant<std::monostate, int, std::string> iris_variant {"some string"};
```





##### 指定类型in_place_index

使用具体的值来初始化variant

```cpp

// 避免被无认为是int
std::variant<long, float, std::string> longFloatString { std::in_place_index<1>, 7 }; 
// 复杂的构造函数传参
std::variant<std::vector<int>, std::string> vecStr { std::in_place_index<0>, { 0, 1, 2, 3 }};
```

- 编译器会帮助 variant 进行匹配（可能存在歧义）

- 为了避免存在类型匹配的歧义 ambiguity

  - 使用`std::in_place_index`来显式指定要匹配的类型。

  - `std::in_place_index`允许创建复杂的类型并将参数传递给该类型对应的构造函数。



##### variant包含自己

骚操作：

```cpp
struct VariantObject;

using VariantList = std::vector<VariantObject>;
using VariantDict = std::unordered_map<std::string, VariantObject>;

struct VariantObject {
    std::variant<std::nullptr_t, int, VariantDict, VariantList> inner;
    std::variant<std::monostate, int, VariantList> inner1;
    std::variant<std::monostate, int, VariantDict> inner2;
}
```





### 运行时类型

##### 类型获取 index()

获取当前使用的类型

```cpp
std::variant<int, float, std::string> intFloatString { "Hello" };

// 2
std::cout << intFloatString.index() << "\n";
```

- `std::variant.index()`
- 通过返回的 `int index`，得知当前什么类型处于激活状态。



##### 类型检查 `std::holds_alternative`

检查当前使用的类型

```cpp
if (std::holds_alternative<std::monostate>(iris))
	std::cout << "the variant holds a std::monostate\n";
else if (std::holds_alternative<float>(intFloatString))
    std::cout << "the variant holds a float\n";
else if (std::holds_alternative<std::string>(intFloatString))
    std::cout << "the variant holds a string\n";  

// 在C++20中 可以在常量表达式中使用std::variant。
constexpr auto dv = foo(1);
static_assert(std::holds_alternative<double>(dv));
```



### 访问

##### 无法直接访问

```cpp
std::variant<int, float, std::string> intFloatString { "Hello" };
// 即使已经明确知道当前激活类型是什么，也无法直接访问
std::string s = intFloatString; 
// error: conversion from 
// 'std::variant<int, float, std::string>' 
// to non-scalar type 'std::string' requested
```

访问varient内部的值，需要使用如下方法：

激活状态（可以传递类型或索引）

类型安全--不允许获取未激活类型的值

|            | `std::get`                               | `std::get_if`                                     |
| ---------- | ---------------------------------------- | ------------------------------------------------- |
|            | 类型 值`std::get<float>(intFloatString)` | 类型index 值引用`std::get_if<1>(&intFloatString)` |
| 激活状态   | 返回**引用**                             | 返回**指针**                                      |
| 非激活状态 | **异常**`std::bad_variant_access`        | 返回**`nullptr`**                                 |



##### std::get

使用 `std::get<Type|Index>(std::variant)`，返回 **引用**

- 所需类型处于激活状态

  返回对应类型的**引用**。

- 所需类型处于非激活状态

  `std::bad_variant_access`异常。

```cpp
// 激活状态 int 
std::variant<int, float, std::string> intFloatString { std::in_place_index<0>, 2};
try 
{
    auto f = std::get<float>(intFloatString); 
    std::cout << "float! " << f << "\n";
}
catch (std::bad_variant_access&) 
{
    std::cout << "our variant doesn't hold float at this moment...\n";
}
```



##### std::get_if

函数`std::get_if<index>(&ref)`。

- 所需类型处于激活状态

  返回指向激活类型的**指针**

- 所需类型处于非激活状态

  不会引发异常，返回`nullptr`。

注意：`std::get`返回对variant的引用，`std::get_if`返回对variant的指针。

```cpp
// 激活状态 int 
std::variant<int, float, std::string> intFloatString { std::in_place_index<0>, 2};

if (const auto intPtr = std::get_if<0>(&intFloatString)) 
    std::cout << "int!" << *intPtr << "\n";
```



##### visitor

访问variant中的值的最重要方法是使用访问者来实现。



### 修改

要求

- 类型安全 -- 不允许获取未激活类型的值
- 对象生存期也在控制范围之内。

四种方法可以variant的当前值，都满足以上要求

##### operator=

赋值操作符

赋值操作符虽然不能访问varient，但是可以修改varient

```cpp
// 激活状态是 string
std::variant<int, float, std::string> intFloatString { "Hello" };
// 赋值操作符
// - 类型 修改为 int, string的析构函数被调用
intFloatString = 10; // we're now an int
```



##### `emplace<index>`

`std::variant.emplace<>{}`

```cpp
intFloatString.emplace<1>(float(10.22));
auto mRef_float = std::get<float>(intFloatString);
std::cout << " after emplace index1-float " << mRef_float << std::endl;
```



##### `std::get` `get_if`

`std::get` 获得引用，然后通过引用给当前激活的类型赋新值

```cpp
// 实际上，这让我对 对象生存期 以及 内存分布 产生了困惑。
// 各个类型的引用似乎可以同时存在
std::variant<int, float, std::string> intFloatString;
intFloatString = 1;
auto mRef_int = std::get<int>(intFloatString);
intFloatString = "Hello ";
std::get<std::string>(intFloatString) += std::string(" World");
auto mRef_str = std::get<std::string>(intFloatString);
std::cout << mRef_str << std::endl; // Hello  World
std::cout << mRef_int << std::endl;	// 1


if (auto pFloat = std::get_if<float>(&intFloatString); pFloat)
    *pFloat *= 2.0f;
```

4. visitor访问器



### std::visitor 

stl 函数`std::visit`。可以在所有的variant参数上调用给定的“访问者”。

```cpp
template <class Visitor, class... Variants>  
constexpr visit(Visitor&& vis, Variants&&... vars);
```

- “接受每个variant中所有可能的类型的可调用对象”

- 必须对所有variant中所有可能的类型对进行重载。

#### lambda 编译期

使用visitor，若采用lambda。可以做到：

- 类型执行相同的逻辑
- 实现静态多态

##### generic

通过variant中的 所有类型 都支持的运算符、函数，来使用传入的变量。

```cpp
// a generic lambda:
// 由于variant中的所有类型都支持<<，可以成功打印
auto PrintVisitor = [](const auto& t) {
    std::cout << t << "\n";
};

std::variant<int, float, std::string> intFloatString { "Hello" };

std::visit(PrintVisitor, intFloatString);
```

```cpp
// 当然，若不加const，visitor可以更改variant当前的值
auto TwiceMoreVisitor = [](auto& t) {
    t*= 2;
};

std::variant<int, float> intFloat { 20.4f };

std::visit(TwiceMoreVisitor, intFloat); // 修改为40.8
std::visit(PrintVisitor, intFloat);		// 打印40.8
```



##### 类型萃取(编译期)

通过类型萃取，根据variant中的类型来指向不同的行为。

值得注意的是：类型萃取发生在 **编译期**，属于静态多态。

```cpp
std::variant<std::monostate, int, float, std::string> intFloatStr {
        std::in_place_index<2>, 20.4f
};

auto TypeTraitVisitor = [](auto& arg) {
    using T = std::decay_t<decltype(arg)>;
    if (std::is_same_v<T, int>) {
        std::cout << "int\n";
    // 使用部分类型特有的 函数/运算符 需要 if constexpr
    } else if constexpr (std::is_same_v<T, float>) {
        std::cout << "float\n";
        arg *= 2;
    } else {
        std::cout << "other than int/float type\n";
    }
};

std::visit(TypeTraitVisitor, intFloatStr);
```



##### 动态多态不行

传入 lambda的参数都 `不再是 variant`，而是 `此时variant中的变量`（至于是 值 引用 右值引用 常量引用... 要看具体情况）。 

lambda传入的不是variant变量。—— 无法使用variant 的判断类型的方法 `.index()`，`std::holds_alternative`。

对于一个普通的变量，因为类型萃取发生在编译期，所以仅仅是静态多态。



#### 仿函数 运行时

lambda 可以实现  执行相同的逻辑/静态多态。

但在大多数情况下，我们希望根据激活类型的不同，来执行一些不同的相应操作。并且是运行时的动态多态。

在仿函数里面重载多个`operator ()`，包含每个variant中所有可能的类型。仿函数/仿函数对象都是ok的

> 符合 "接受每个variant中所有可能的类型的可调用对象" 的定义

- 允许variant存在 `std::monostate`，仿函数包含对应的重载operator()即可。

- 可以包含多余的`operator ()`重载（该variant没有的类型）。

```cpp
struct MultiplyVisitor
{
    float mFactor = 9.9;

    MultiplyVisitor(float factor) : mFactor(factor) { }
    MultiplyVisitor() {}

    void operator()(std::monostate t) const {
        std::cout << "std::monostate type, mFactor = " << mFactor << std::endl;
    }

    void operator()(std::string& ) const {
        std::cout << "string called , mFactor = " << mFactor  << std::endl;
    }

    void operator()(int& i) const {
        std::cout << "int called , mFactor = " << mFactor  << std::endl;
        i *= static_cast<int>(mFactor);
    }

    void operator()(float& f) const {
        std::cout << "float called , mFactor = " << mFactor << std::endl;
        f *= mFactor;
    }
};
```

```cpp
std::variant<std::monostate, int, float, std::string> intFloatStr {
    std::in_place_index<2>, 20.4f
};

// 重载 operator() 的仿函数
std::visit(MultiplyVisitor(), intFloatStr);
// 只要仿函数重载了对应的operator()，monostate也是允许的
intFloatStr = std::monostate();
std::visit(MultiplyVisitor(0.5f), intFloatStr);

// 也可以是仿函数对象
intFloatStr = "string sth";
MultiplyVisitor mVisitor = MultiplyVisitor(99.0f);

std::visit(mVisitor, intFloatStr);
```



#### lambda 支持缺省类型

对于lambda而言

- 可以在一个位置编写多个lambda来匹配variant中的所有类型

- 目前，它不是库的一部分，但可能的代码如下所示：



## std::optional

```cpp
#include <optional>
```

### 概念

#### **用途**

只有两种可能的`std::variant`简化版 —— 规定的类型 或 `std::nullopt`。

适用的情况：值已经确实是某一特定类型，关心它是否存在。

1. 避免使用 `空指针`或`特殊值` 来表示无值
2. 提高代码的简洁性和安全性

#### **具体使用场景**

可选状态：不确定某个对象 处于/不处于某种状态。

> 例如，一个购物车可能包含一个可选的运费，`std::optional<double>`表示是否计算运费。

宽泛来说，用在函数中：

- 返回值：函数可能 返回&不返回 值

- 参数：接受/忽略 该参数的值。

容器类的元素：
- 作为容器类（如std:vector、std:list等）的元素类型
- 存储可能不存在的值。方便地处理容器中的空值。

异步编程：
- 异步操作的 计算结果/状态
- 应该是一个确定的类型，但可能暂时还没有结果（空值）。



### 构建方式

#### 构造函数

默认构造、拷贝构造、移动构造

```cpp
// 默认构造
// - 创建一个不包含值的std::optional对象
// - 需要延迟初始化 或者 天然的表示一个可能不存在的值时
std::optional<double> opt;

std::optional<double> a(10.09); // 创建一个包含值10.09的std::optional对象

// 拷贝构造
std::optional<double> b(a); // b的值于a相同

// 移动构造
std::optional<double> c(std::move(a)); // 现在 a 为nullopt
// std::string 移动构造，用推导指引拾取类型
std::optional o6(std::string{"deduction"});
```



#### std::in_place_t

用来指示原位构造对象：

- 在`std::optional`对象的存储空间中直接构造值

  避免了不必要的拷贝或移动操作。

- 初始化列表`std::initializer_list` / 可变参数

  传递给 `std::optional`的构造函数。

```cpp
// 调用 std::string( initializer_list<CharT> ) 构造函数
std::optional<std::string> o4(std::in_place, {'a', 'b', 'c'});

// 调用 std::string( size_type count, CharT ch ) 构造函数
std::optional<std::string> o5(std::in_place, 3, 'A');
```



#### std::make_optional

可以从右值，可变参数和std::initializer_list等多种方式用std::make_optional构造出std::optional对象

```cpp
// 右值
std::string str{"hello world\n"};
auto op1 = std::make_optional<std::string>(std::move(str));
std::cout << op3.value_or("empty value\n");

// 可变参数
auto op2 = std::make_optional<std::vector<int>>(5, 2);

// 初始化列表 std::initializer_list
// 用花括号初始化器列表列表初始化一个对象，其中对应构造函数接受一个std::initializer_list 参数
auto op3 = std::make_optional<std::vector<char>>({'a','b','c'});
std::cout << "op1: ";
for (char c: op1.value()){
    std::cout << c << ",";
}
```



### 观察器

判断是否有内容

- `operator bool`
- `bool has_value()`函数

指针、引用：

- `operator->`返回所含值的指针；
- `operator*`返回所函数的引用，

访问

- `value()`函数，

  - 含值返回  `值引用`
  - 不含值 throw `bad optional access`

- `value_or()`函数，
- 含值返回  `值`
  
- 不含值  返回 `default_value`

```cpp

std::optional<int> opt = {};
try {
    int n = opt.value();
} catch(const std::exception& e) {
    std::cout << e.what() << '\n';
}


std::cout << opt.value_or("(none)");

```



### 修改器

operator=、emplace、reset、swap

- emplace

在optional对象中就地构造值。若 std::optional 已经含值，析构原有对象后再次原地构造。

- swap

交换内容，如果内部有值，这先析构内部值，再交换值。

- reset

重置对象 到`std::nullopt`，若 std::optional 含值，则如同用 value().T::~T() 销毁此值。否则无效果。

```cpp
// 构造函数
std::optional<MyCLass> s1 = MyClass(1), s2;

// operator=
s2 = s1;		// 拷贝赋值
s1 = MyClass(2);		// 移动构造：构造，再移动 

// emplace
// - 在optional对象中就地构造一个值
std::optional<int> a;
a.emplace(MyClass(3));
a.emplace(MyClass(4));	// 析构原先的值，再就地构造新值

a.reset();	// std::nullpot

```



### 单子操作 Cpp23

transform 从 `普通值/optional` 到 `std::optional`

and_then 从 `optional` 到 `使用其作为参数的函数 返回值 optional`

or_else 若optional为空，则返回指定函数的返回值。

| 单子操作  | 普通值 | std::nullopt |
| --------- | ------------------------------------------------------------ | ------------------------------------ |
| transform | 函数的返回值 对应的 std::`optional`，函数参数带入 原optional | 空的 std::`optional`                 |

| 单子操作  | std::`optional` 含值                                         | std::nullopt                         |
| --------- | ------------------------------------------------------------ | ------------------------------------ |
| and_then  | 函数的返回值，函数参数带入 原optional                        | 空的 std::`optional`                 |
| or_else   | 所含值的 std::`optional`                                     | 函数的返回值（option不作为参数输入） |

```cpp
#include <charconv>
#include <iomanip>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
 
std::optional<int> to_int(std::string_view sv)
{
    int r {};
    auto [ptr, ec] { std::from_chars(sv.data(), sv.data() + sv.size(), r) };
    if (ec == std::errc())
        return r;
    else
        return std::nullopt;
}
 
int main()
{
    using namespace std::literals;
    const std::vector<std::optional<std::string>> v
    {
        "1234", "15 foo", "bar", "42", "5000000000", " 5", std::nullopt, "-43"
    };
 
    for (auto&& x : v | std::views::transform(
        [](auto&& o)
        {
            // 调试打印输入的 optional<string> 的内容
            std::cout << std::left << std::setw(13)
                      << std::quoted(o.value_or("nullopt")) << " -> ";
 
            return o
                // 若 optional 为空则转换它为持有 "" 字符串的 optional
                .or_else([]{ return std::optional{""s}; })
                // 拉平映射 string 为 int （失败时产生空的 optional）
                .and_then(to_int)
                // 映射 int 为 int + 1
                .transform([](int n) { return n + 1; })
                // 转换回 string
                .transform([](int n) { return std::to_string(n); })
                // 以 and_than 替换，并用 "NaN" 变换并忽略所有剩余的空 optional
                // and_then and ignored by transforms with "NaN"
                .value_or("NaN"s);
        }))
        std::cout << x << '\n';
}
```





## std::any

### 概念

 std::any的设计目标：

- 提供一种类型安全且易于使用的方式来在运行时处理各种类型的数据
- 任何错误的类型转换都会在运行时抛出异常。

是什么：

- std::any是一种容器，不是模版类
  - **用于**任何 `可拷贝构造类型的单个值`  的类型安全**容器**。
  - 一个动态类型变量，可以存储任何类型的值
- 只能容纳一个元素，可以是任意的类型
  - 基本数据类型（int，double，char，float...）

  - 复合数据类型（类、结构体）。
- 值类型

  - 可查询类型：当前保存的是哪种类型的值
  - 可更改类型：更改后仍具有类型安全性。

缺点

- 运行时并不知道它存储的数据的具体类型，

  - 需要显式地进行类型转换。

  - 这可能会使代码变得复杂和难以理解。

- 性能不如其他类型，运行时需要进行类型检查和类型转换。



### 创造

#### 构造函数

在声明此类型的对象时，不需要指定可能的类型。

```cpp
std::any m_any_a;
```

当然，也可以赋初值

```cpp
std::any m_any_int = 32; //type: int
std::any m_any_str = "wegrthweg"; //type : const chr*
```

保存不同的类型：`std::in_place_type`指明类型

```cpp
std::any m_any_str{std::in_place_type<int>, 420};
std::any m_any_int{std::in_place_type<std::string>, "asdfbsrghtr34"};
```

多个参数初始化

- 显式的创建对象
- 或 使用`std::in_place_type`指明类型

```cpp
// 创建对象
std::any a1{MyClass{6.0, 2.0}};
// std::in_place_type作为第一个参数
std::any a2{std::in_place_type<MyClass>, 6.0, 2.0};
// std::in_place_type + 初始化列表?
std::any a2{std::in_place_type<set<int>>, {3, 7, -1, -16, 1, 100}};
```

初始化器列表，后附参数?

```cpp
auto func = [] (int x, int y) { return std::abs(x) < std::abs(y);};
 
std::any m_any_set{
    std::in_place_type<std::set<int,decltype(func)>>, 
    {3, 7, -1, -16, 1, 100}, 
    func
};
```

lambda放入any需要make_any

```cpp
// 把 lambda 放入 std::any 不用make_any（失败）。
std::any a2 = [] { std::cout << "Lambda #1.\n"; };
std::cout << "a2.type() = \"" << a2.type().name() << "\"\n";
// any_cast 转型到 <void(void)> 
// 但实际类型是 ~ main::{lambda()#1} ，且每个 lambda 唯一。所以出错
try {
    std::any_cast<std::function<void(void)>>(a2)();
}
catch (std::bad_any_cast const& ex) {
    std::cout << ex.what() << '\n';
}

// 尝试
std::any any_lambda {
    std::in_place_type<std::function<void(void)>>,
    [] { std::cout << "Lambda #1.\n"; }
};
```



#### std::make_any

需要显式指定初始化的类型。

```cpp
auto a0 = std::make_any<std::string>(
    "Hello, std::any!\n"
);
std::cout << std::any_cast<std::string&>(a0);
std::cout << a0.type().name() << "\"\n";

// 将 lambda 放入 std::any 使用 make_any
auto a_lambda = std::make_any<std::function<void(void)>>(
    [] { std::cout << "Lambda #2.\n"; }
);
std::any_cast<std::function<void(void)>>(a_lambda)();
```

定义如下

```cpp
template< class T, class... Args >
std::any make_any( Args&&... args );
// 等价于
return std::any(
    std::in_place_type<T>, 
    std::forward<Args>(args)...);
```

```cpp
template< class T, class U, class... Args >
std::any make_any( std::initializer_list<U> il, Args&&... args );
// 等价于
return std::any(
    std::in_place_type<T>, 
    std::initializer_list<U> il, 
    std::forward<Args>(args)...);
```



#### operator=

```cpp
//[1]
any& operator=( const any& rhs );
//[2]
any& operator=( any&& rhs ) noexcept;
//[3]
template<typename ValueType>
any& operator=( ValueType&& rhs );
```



### 检查

#### 是否有值 `.has_value`

检查对象是否含有值，

- 若实例含值则为 true ，
- 否则为 false 。示例如下：



#### 所含类型 `.type`

查询所含类型，若实例

- 非空，所含值的 `typeid`
- 空， `typeid(void)` 









### 访问

要访问any包含的值，必须使用std::any_cast<>将其转换为其类型。

std::any的构造函数使用了std::decay_t。会导致例如 

- 数组`T[N]` 退化成`T*`
- 函数 退化成 函数指针

#### 值/引用

##### 三个重载

```cpp
template< class T >
T any_cast( const any& operand );

template< class T >
T any_cast( any& operand );

template< class T >
T any_cast( any&& operand );
```

```cpp
// 创建并返回临时对象
std::any_cast<std::string>(a);
// 返回引用
//  - 可以修改该值
//  - 避免了创建临时对象
std::any_cast<std::string&>(a);
// 返回常引用
//  - 可以避免误修改
//  - 避免了创建临时对象
std::any_cast<const std::string&>(a); // read-access by reference
```

##### 匹配失败

如果转换失败，将抛出std::bad_any_cast异常

```cpp
try {
    auto s = std::any_cast<std::string>(a);
    ...
}
catch (std::bad_any_cast& e) {
    std::cerr << "EXCEPTION: " << e.what() << '\n';
}
```





#### 指针

为std::any对象的地址调用std::any_cast

```cpp
template< class T >
const T* any_cast( const any* operand ) noexcept;

template< class T >
T* any_cast( any* operand ) noexcept;
```

类型是否匹配

- 匹配：返回相应的地址指针

- 不匹配：返回nullptr:

```cpp
auto p = std::any_cast<std::string>(&a);
if (p) {
    ...
}
```



### 修改

emplace

更改所含对象，直接构造新对象

reset

销毁所含对象。

swap

交换两个std::any。





# 说明符&限定符

分为

- 存储说明符 storage class specifier 
- cv-限定符 cv-qualifier

## 存储说明符

存储说明符 storage class specifier 

同一声明中不能使用多个说明符，但thread_local可与static或extern结合使用。

存储方式

| 存储描述                           | 持续性 | 作用域 | 链接性 | 关键字   |
| ---------------------------------- | ------ | ------ | ------ | -------- |
| 自动（局部变量）                   | 自动   | 代码块 | 无     |          |
| 寄存器（局部变量）                 | 自动   | 代码块 | 无     | register |
| 静态，无链接性（静态局部变量）     | 静态   | 代码块 | 无     | static   |
| 静态，外部链接性（全局变量）       | 静态   | 文件   | 外部   |          |
| 静态，内部链接性（静态全局变量前） | 静态   | 文件   | 内部   | static   |

- auto C++11中不再是说明符

  > 用于自动类型推断

- register

  > 声明中指示寄存器存储，C++11中只是显式地指出变量是自动的

- static

  > 表示内部链接性（全局变量）/存储持续性为静态（局部变量）

- extern

  > 引用声明，声明引用在其他地方定义的变量。

- thread_local C++11新增

  > 指出变量的持续性 与其所属线程的持续性相同
  >
  > 其之于线程，就和静态变量之于程序一样

- mutable

  > 用它指出，即使结构（或 类）变量为const，它的某个成员也可以被修改。
  >
  > ```cpp
  > struct data
  > {
  > mutable char name[20];  
  > int num;
  > };
  > const data n1={"ok",1};
  > strcpy(n1.name,"joyboy"); // 对的 因为mutable
  > n1.num++; // 错 因为const
  > ```



### register

关键字`register`，声明寄存器变量，指示寄存器存储。

- 最初由C语言引入。
  - 它建议编译器使用CPU寄存器来存储自动变量(只能用于局部变量)。
  - 旨在提高访问变量的速度，提示表明变量用得很多。

- C++11中，

  - 这种提示功能也失去了，
    - 关键字`register`只是显式地指出变量是自动的(即局部变量)，
    - 只能用于原本就是自动的变量(即局部变量)。

  - 限制使用它的唯一原因

    指出程序员想使用一个自动变量，这个变量的名称可能与外部变量相同。

    这与auto以前的用途完全相同。

  - 保留关键字`register`的原因：

    避免使用该关键字的现有代码非法。

```c
register int count_fast_int;
```





### static

#### 关键字含义

关键字的含义取决于上下文，可以称之为关键字重载。关键字static的两种用法：

##### 存储持续性

表示**存储持续性**（局部变量）

- 用于局部声明，指出变量是无链接性的静态变量



##### 内部链接性

表示**内部链接性**（全局变量，成员函数）

> 类作用域意味着不能从外部直接访问类的成员，包括共有成员函数。声明类只是描述了对象的形式，并没有创建对象。创建对象前，没有用于存储值的空间。
>

- 变量

  - 用于代码块外的声明（变量已经是静态持续性了）

  - 无论创建多少对象，程序只创建一个静态类变量副本。

    - 与其他静态变量存储在 `data`静态全局区，而不是存储在对象中。

    - 作用域：一个静态成员被类的所有对象共享。

- 静态成员函数

  - 如果把函数成员声明为静态的，就可以把函数与类的任何特定对象独立开来。
  - 只要使用类名加范围解析运算符 **::** 就可以访问。即使在类对象不存在的情况下也能被调用，
  - 只能访问静态成员数据、其他静态成员函数和类外部的其他函数。
  - 与普通成员函数的区别：
    - 没有 this 指针，只能访问静态成员（包括静态成员变量和静态成员函数）和类外部的其他函数。
    - 普通成员函数有 this 指针，可以访问类中的任意成员；





#### 进程上下文

- `text` 代码 (二进制)  只读 **文字常量区、代码区**
- `data` 全局和静态变量数据 **静态全局区**
- `stack 栈`  存放**局部变量，函数返回值**  FIFO
- ==`heap 堆`    程序运行时的**动态内存分配**==
- `PCB 进程控制块` 包含进程信息(控制信息等)









#### 初始化

##### 初始化分类

动态初始化：意味着变量将在编译后初始化。

静态初始化：意味着编译器处理文件（翻译单元）时初始化变量。

- 常量表达式初始化 

- 零初始化

  如果没有显式地初始化静态变量，编译器将静态变量的所有位都设置为0。这种变量被称为**零初始化的**（zero-initialized）。

  > 默认情况下，静态数组和结构将每个元素或成员的所有位都设置为0。

  对于标量类型，零将被强制转换为合适的类型。

  - 指针变量将被初始化为相应的内部表示，因为尽管用`0`表示空指针，但其内部可能采用非零表示。
  - 结构成员被零初始化，填充位都被设置为零。

  这也是为什么，全局变量，全局静态变量，局部静态变量未初始化时，默认为0。



##### 注意事项

应该声明在`.h类声明文件`中，初始化在`.cpp类方法文件`中。

不能在类声明中初始化 静态成员变量。

- 为什么？
  - 如果在头文件中进行初始化，将出现多个初始化语句副本，从而引发错误。

- 例外情况：
  - 静态数据成员 为整型或枚举型const。
- 静态成员变量
  - 对于静态成员变量，在类声明之外，使用范围解析运算符 `::` 来初始化

- `int Myclass::age = 0;`

  - 初始化时指出了类型，作用域运算符，没有使用static

  - 这是因为声明已经描述了如何分配内存，但是还没有完成分配内存的动作。
  - 静态类成员是单独存储的，而不是对象的组成部分。

```cpp
// .h 类定义文件中:
class Myclass{
    pubilc:
    static int age; // 静态变量
    int user; // 实例变量
    public:
    func(){
        int local; //局部变量
    }
}
// .cpp类方法文件
int Myclass::age = 10; //初始化静态变量
```





### extern

> 引用声明，声明引用在其他地方定义的变量。

##### 语言链接性

extern只用在变量声明，因为变量的定义后，可能压根就没有赋值

```cpp
// extern只用在变量声明
extern int a; // 区分int a=1；
// 函数和类，不需要，可以通过;还是{}进行区分
```





###### **`extern` 非 `const` 全局链接**

链接器在全局变量声明之前看到 **`extern`** 时，它会在另一个转换单元中查找定义。

默认情况下，全局范围内的非 **`const`** 变量声明为 external。 仅将 **`extern`** 用于未提供定义的声明。

```cpp
//fileA.cpp
int i = 42; // declaration and definition

//fileB.cpp
extern int i;  // declaration only. same as i in FileA

//fileC.cpp
extern int i;  // declaration only. same as i in FileA

//fileD.cpp
int i = 43; // LNK2005! 'i' already has a definition.
extern int i = 43; // same error (extern is ignored on definitions)
```

###### **`extern` 全局 `const` 链接**

默认情况下，**`const`** 全局变量具有内部链接。 如果希望变量具有 external 链接，请将 **`extern`** 关键字应用于定义以及其他文件中的所有其他声明：

```cpp
//fileA.cpp
extern const int i = 42; // extern const definition

//fileB.cpp
extern const int i;  // declaration only. same as i in FileA
```





```cpp
extern constexpr int x = 10; //error LNK2005: "int const x" already defined
```

如果头文件包含声明 `extern constexpr` 的变量，必须将它标记为 `__declspec(selectany)`，以便正确组合其重复声明：

```cpp
extern constexpr __declspec(selectany) int x = 10;
```





##### 名称修饰 extern "C"

C++为了能够实现重载函数，会执行 名称修饰 name decoration 或 名称矫正 name mangling。

它根据函数原型中指定的形参类型对每个函数名进行加密。这也是为什么C++中 不能直接调用C函数的原因(需要extern C)。

由于 c++中需要支持函数重载，所以 c 和 c++中对同一个函数经过编译后生成的函数名是不相同的

> c 函数: void MyFunc(){} ,被编译成函数: MyFunc 
>
> c++函数: void MyFunc(){},被编译成函数: _Z6Myfuncv

extern "C"的主要作用就是为了实现 c++代码能够调用其他 c 语言代码。加上 extern "C"后，这部分代码编译器按 c 语言的方式进行编译和链接，而不是按 c++的方式

```cpp
extern "C" void myfunc(int); // C
extern void myfunc(int); // C++
extern "C++" void myfunc(int);//C++
```



```c++
//h文件中 MyModule.h
#ifndef MYMODULE_H 
#define MYMODULE_H 
#include<stdio.h> 

#if __cplusplus 
extern "C"{ 
#endif
    void func1(); 
    int func2(int a,int b); 
#if __cplusplus 
}
#endif 
#endif

//c文件中 MyModule.c
#include"MyModule.h" 
void func1(){ 
    printf("hello world!"); 
}
int func2(int a, int b){
    return a + b; 
}

// Cpp文件中 TestExternC.cpp
#define _CRT_SECURE_NO_WARNINGS 
#include<iostream> 
using namespace std;
//  声明c文件中的函数
extern "C" void func1();
extern "C" int func2(int a, int b);
int main(){ 
    func1(); 
    cout << func2(10, 20) << endl; 
    return EXIT_SUCCESS; 
}
```





### thread_local

C++11新增

> 指出变量的持续性 与其所属线程的持续性相同
>
> 其之于线程，就和静态变量之于程序一样



### mutable

> 用它指出，即使结构（或 类）变量为const，它的某个成员也可以被修改。







## cv-限定符

cv-限定符 cv-qualifier (cv 表示的就是c-const v-volatile)

- const
- volatile

### volatile

关键字`volatile`的作用是为了改善编译器的优化能力，用于可能快速变化的变量。

> 如果不声明`volatile`，则编译器可能会进行以下优化：
>
> 编译器发现程序在几条语句中两次使用了某个变量的值，则编译器可能不是让程序查找这个值两次，而是将这个值缓存到寄存器中。这种优化假设变量的值在这两次使用之间不会变化。

关键字`volatile`表明，即使程序代码没有对内存单元进行修改，其值也可能发生变化。（很离谱）

> 例如将一个指针指向某个硬件为止，其中包含了来自串行端口的时间或信息。
>
> 硬件（不是我们的程序）可能修改其中的内容，或者两个程序可能互相影响，共享数据。

### const

C++中（不是C语言）const对默认存储类型稍有影响。

默认情况下，全局变量的链接性为外部的，但const全局变量的链接性为内部的。（性质类似于使用了static说明符，静态局部变量）

允许多个文件中，同时分别定义同名的 const类型的全局变量。每个文件都有自己的一组常量，而不是所有文件共享一组常量。

这就是能够将常量定义放在头文件中的原因。

> C语言中，这样的const声明的链接性像常规变量(其他全局变量)那样是外部的，将违反但定义规则。



#### 作用

##### 函数参数

由于传递的是地址，使用原始数据会增加破坏数据的风险。

const限定符提供了解决这个问题的办法。声明形参时，使用关键字`const`

```cpp
void show_array(const double ar[], int n);
void show_array(const double *ar,  int n);
```

将指针参数声明为指向常量数据的指针：

- 避免由于无意间修改数据而导致的编程错误
- 使用const使得函数能够处理const和非const实参，否则只能接受非const数据

```cpp
const int arr[6] = {1,2,3,4,5,6};
// 不允许
int sum(int arr[], int n);
int sum(arr,6);  
// 允许
int sum(const int arr[], int n);
int sum(arr,6);  
```



##### 替代宏

用const替代宏

```cpp
#define MONTHS 12
const int Months = 12;
```

- const能够明确地指明类型
- C++地作用域规则将定义限制在特定的函数或文件中
- 可以将const用在更复杂的类型(数组，结构)



#### 什么可以修改

const 用于指针有两种方式:

- 让指针指向一个常量对象，防止使用该指针来修改所指向的值。

> 指向const的指针

- 将指针本身声明为常量，防止改变指针指向的位置。

> const指针

###### 指针指向的内容 `const int *pt`

a pointer to const int **不能改值，能改地址**

- const**能**防止通过`*pt`修改 **pt指向的值**，
- const**不能**防止修改pt指向的地址，可以**为新的变量赋值**，**再赋**新地址给pt。

```cpp
int num = 39;
int n = 80;
const int *pt = &num;

pt = &n; // 合法
*pt = 80; // 非法
```



###### 指针的指向 `int * const pr`

a const pointer to int **能改值，不能改地址**

无法修改指针指向的值`*pr`

- figer只能指向num，不可以赋给它新地址
- 允许使用`*pr`修改num

```cpp
int num = 3;
int n = 10;
int * const pr = &num;
*pr = 20; // 合法
pr = &10; // 非法
```





#### 使用场景

##### const成员函数

###### 问题、方法

如何确保函数不会修改调用对象？

只要类方法不修改调用对象，就应将其声明为`const`。

```cpp
const Myclass class04 = Myclass(1.2,3);
/* 函数此时无法调用，因为无法确保调用对象不被修改 */
// 尽管该方法没有对调用对象进行任何修改
class04.show(); 
```

解决方法：`const`放在函数的括号后面，进行声明、定义。

```cpp
void show() const; // 类中函数原型

/* C++程序 */
void Myclass::show() const
{
    cout << val;
}// 函数定义
class01.show(); // 函数调用

/* C-风格 */
void show(const Myclass * this) 
{
    cout << this->val;
}// 函数定义
show(&class01); // 函数调用
```

###### 示例



```cpp
const Myclass & innerfunc(const Myclass & s) const;

class01 = class02.innerfunc(class03);
```



该函数 **隐式的**访问一个对象，**显式的**访问一个对象，**返回**一个对象的引用。

- 开头的`const` ： 返回类型为，`const`Myclass的引用。 （返回值`class01`）
- 括号中的`const`： 该函数不会修改被显式地访问的对象。（对应`class03`）
- 括号后的`const`： 该函数不会修改被隐式地访问的对象。（对应`class02`，不能使用`this`修改对象的值）

对应的函数定义。：

`const Myclass & Myclass::innerfunc(const Myclass & s) const{}`



##### 指针

将指针参数声明为指向常量数据的指针：

- 避免由于无意间修改数据而导致的编程错误
- 使用const使得函数能够处理const和非const实参，否则只能接受非const数据

条件允许，则应将指针形参声明为指向const的指针。

const 用于指针有两种方式:

- 让指针指向一个常量对象，防止使用该指针来修改所指向的值。
- 将指针本身声明为常量，防止改变指针指向的位置。

###### 取地址

可以使用强制类型转换`const_cast`运算符，突破限制。

常规变量的地址 赋给 常规指针

```cpp
int age = 10;
int *p = &age;
```

常规变量的地址 赋给 const指针

```cpp
int age = 10;
const int *p = &age;
```

const变量的地址 赋给 const指针

```cpp
const int age = 10;
const int *p = &age;
```

const变量的地址 赋给 常规指针 (错误)

但可以使用强制类型转换`const_cast`运算符，突破限制。

```cpp
const int age = 10;
int *p = &age;   // 错误
```



###### 一级指针

数据类型本身不是指针，

- 可以将 **const数据** 或 **非const数据的地址**，赋给指向const的指针，
- 只能将**const数据的地址**赋给const指针。

一级间接关系，将非const指针赋给const指针是可以的。该技术可以用于函数定义和函数原型(元素类型是基本类型)。

```cpp
int age = 39;
const int *pt = &age;

// 不能再修改pt指向的位置
*pt +=1; // 错误
cin >> pt; // 错误
// 可以修改pt指向位置的内容
*pt = 20; // 错误，对pt而言值是常量
age = 20; // 正确，对age而言值不是常量
```

###### 二级指针

一级间接关系，将非const指针赋给const指针是可以的

然而进入二级间接关系，与一级间接关系一样，将const非const混合的指针赋值方式将不再安全。



```cpp
const int n = 13;
int *p1;
const int **pp2;

pp2 = &p1; // 不符合规范，只是做演示
*pp2 = &n;
*p1 = 10;  // 成功修改const int n。 这不符合规范
```

将非const地址 `&p1` 赋给 const指针 `pp2` ，因此可以使用p1修改const数据。



##### 数组

将指针参数声明为指向常量数据的指针：

- 避免由于无意间修改数据而导致的编程错误
- 使用const使得函数能够处理const和非const实参，否则只能接受非const数据

条件允许，则应将指针形参声明为指向const的指针。

> 如果只有一层间接关系，则该技术可以用于函数定义和函数原型(元素类型是基本类型)。
>
> 如果数组元素是指针，则不能使用const

const 类型的数组，禁止将常量数组的地址，赋给非常量指针。

不能将数组名作为参数传递给使用非常量形参的函数。

```cpp
const int arr[6] = {1,2,3,4,5,6};
// 不允许
int sum(int arr[], int n);
int sum(arr,6);  
// 允许
int sum(const int arr[], int n);
int sum(arr,6);  
```













#### extern const

如果出于某种原因，程序员希望某个常量的链接性为外部的。

可以使用`extern`关键字来覆盖默认的内部链接性。

`extern const int stated = 50;`

这种情况下，必须在所有使用该常量的文件中使用extern关键字来声明。

在使用该变量的其他文件中必须使用extern，而且只有一个文件可对其进行初始化。



### constexptr

见 C++元编程.md





# 类型转换运算符 xx_cast

C++采取严格地限制允许的类型转换，并添加了4个类型转换运算符，使转换过程更规范。



## general 显式转换

`static_cast` 一般用来转换==内置的基本数据类型==  

`dynamic_cast` 包含虚函数的==类层次结构==中，进行向上转换，而不允许其他转换。

`const_cast` 一般用来转换==加const和去除const==

`reinterpret_cast` 一般用来==转指针== 整数和指针之间都可以转换







## static_cast 基本数据类型

### 内置类型

一般用来转换==内置的基本数据类型==  

`static_cast <type-name> (expression)`

1. static_cast可以用来转换基本的内置数据类型 int char double... 
2. static_cast不能转换没有发生继承关系之间的类 
3. static_cast可以用来转换发送继承关系之间的类，但是不保证安全性 
4. 不能用来转换指针
5. 无需进行类型转换，枚举值enum可以转换为int，double转换成int，float转换成long...

```c++
int a = 1; 
char b = 2;
double c = 3.14;
a = static_cast<int>(b);
a = static_cast<int>(c); 
c = static_cast<double>(a)
```

static_cast不能转换没有发生继承关系之间的类 
		如果两个类之间发生了继承关系,可以类型转换 但是static_cast不会保证转换的安全性.

```c++
class A
{
    public:
    int a;
};
class B:public A 
{ 
    public: 
    int b;
};

主函数中
A *p1 = new A;
B *p2 = new B; 

p1 = static_cast<A*>(p2);//子转父 向上转换 是安全的
p2 = static_cast<B*>(p1);//父转子 向下转换 是不安全的 36 }
```

### 右值

有时我们也希望，将移动语义用于**左值**。（使用移动构造函数 或移动赋值运算符）

类 T 的移动赋值运算符是名为 `operator=`的非模板非静态成员函数，它接受恰好一个 `T&&、const T&&、volatile T&& 或 const volatile T&&`类型的形参。

> 创建一个指定对象的数组`choices`，选择其中一个对象`choices[2]`使用后丢弃数组。
>
> 则如果使用移动构造函数 或移动赋值运算符，来保留选定的对象会更加的理想。

- 使用运算符`static_cast<>`将对象的类型强制转换为 右值`Useless &&`

```cpp
Myclass class01, class02, class03;
class03 = class01 + class02; // 右值引用，直接调用移动赋值运算符
class03 = class01; // 左值引用
class03 = static_cast<Myclass &&> class01; // 左值引用，但被强制类型转换。
```

- 头文件`utility`，函数`std::move()`。(更简单)

```cpp
Myclass class01, class02, class03;
class03 = class01 + class02; // 右值引用，直接调用移动赋值运算符
class03 = class01; // 左值引用
class03 = std::move(class01); // 左值引用，但被转换为右值Myclass &&。
```



## dynamic_cast 继承 安全

一般用来转发生继承关系之间的==自定义的数据类型==。包含虚函数的类层次结构，只允许向上转换。

1. **不能**用于转换**基本的数据类型** 
2. 可以用于转换发送**继承关系**之间的类,**保证转换是安全的** 子转父是可以的 
3. 如果发生了==多态==,子转父和父转子总是安全的 

```c++
class A
{
    public:
    int a;
};
class B:public A 
{ 
    public: 
    int b;
};
主函数中
A* p1 = new A;
B* p2 = new B;
p1 = dynamic_cast<A*>(p2); // 子转父 是允许的
//p2 = dynamic_cast<B*>(p1); // 父转子 非法
```



## const_cast - cv限定符

只有一种用途，改变值`const`或`volatile`。其语法与`dynamic_cast`运算符相同。

`const_cast <type-name> (expression)`

提供该运算符的原因，有时可能需要这样一个值：大多数时候是**常量**，有时有是**可以修改的**。

> 虽然也可以使用通用类型实现，但通用转换也可能同时改变类型。

const_cast 一般用来转换==加const和去除const==

```c++
int* p1 = NULL;
const int *p2 = NULL;
int* p3 = NULL;
const int *p4 = NULL;
p1 = const_cast<int*>(p2);
p4 = const_cast<const int*>(p3);
```

`const_cast`不是万能的。它可以修改指向一个值的指针，但修改`const`值的结果是不确定的。

> 指针可以删除`const`特征，一些操作不会再报错，但是
>
> 仅当指向的值本身不是`const`时，才行。否则，虽然不报错，但等于没操作。



## reinterpret_cast

reinterpret_cast 运算符，用于天生危险的类型转换。

不允许删除`const`，但会执行其他不那么好的操作。

有时程序员必须执行一些依赖于实现的、令人生厌的操作。对于这些操作，`reinterpret_cast`运算符可以简化对这种行为的跟踪工作

`reinterpret_cast <type-name> (expression)`

重新解释转换 `reinterpret_cast` 一般用来==转指针== 整数和指针之间都可以转换(可以在指针和引⽤之间进⾏肆⽆忌惮的转换)

- 适用于依赖实现的底层编译技术，是不可移植的。例如，不同系统的存储多字节整型时，可能以不同的顺序存储其中的字节。

```cpp
struct dat { short a; short b; };
long value = 0xA224B118;
dat * pd = reinterpret_cast< dat * > (&value);
cout << hex << pd->a; // 展示value的 前两个2字节
```

- 将指针类型转换为足以存储指针表示的整型（不能是更小的整型或浮点型）

```cpp
// char类型太小，不足以存储指针。
char ch = char (&d);
```

- 不能将函数指针转换为数据指针。

```c++
// 静态转换 不是指针
a = static_cast<int>(b);
a = static_cast<int>(c); 
c = static_cast<double>(a);
// 重新解释转换
int *p = NULL; 
char *p2 = NULL;
p = reinterpret_cast<int *>(p2); 
p2 = reinterpret_cast<char *>(p); 
int c = 0; 
c = reinterpret_cast<int>(p2);

```



# 类型识别RTTI(运行时)

### 概念

RTTI 运行阶段类型识别，Runtime Type Identification 的简称。旨在为程序**在运行阶段确定对象的类型**提供一种标准方式。

> 很多类库为其类对象提供了实现RTTI这种功能的方式，但C++内部不支持。一些老式实现不支持RTTI。
>
> 因此，创建一种RTTI语言标准将使得未来的库能够彼此兼容。

适用场景

- 只能将RTTI用于包含虚函数的类层次结构。

用途（为什么要RTTI）

- RTTI可以帮我们知道，**指针指向**的具体是**哪个派生类对象**（或者是基类对象）。

- 对于 **类层次结构** ，其中的类都是从同一个基类派生而来的，则可以让基类指针指向其中任何一个类的对象。RTTI可以帮我们知道，**指针指向**的具体是**哪个派生类对象**（或者是基类对象）。
- 类层次结构中我们调用的类方法，可能是类层次结构中所有成员都拥有的**虚函数**，并不需要真正知道对象的类型。

- 但，我们希望**确保**调用类方法的正确版本。RTTI为以下两种情况提供解决方案。

  - 派生类可能包含不是继承而来的方法。只有某些类型的对象，才可以使用该方法。

  - 出于调试的目的，想跟踪生成的对象的类型。

在C++中，为了支持RTTI提供了两个操作符：dynamic_cast和typeid

- dynamic_cast
  - 允许运行时刻进行类型转换，从而使程序能够在一个类层次结构中安全地转化类型，
  - 与之相对应的还有一个非安全的转换操作符static_cast，因为这不是本文的讨论重点，所以这里不再详述，感兴趣的可以自行查阅资料。

- typeid：返回一个指出对象的类型的值

  - C++的关键字之一，等同于sizeof这类的操作符。

  - 返回结果是名为type_info的标准库类型的对象的引用

    `type_info`结构 ：存储有关特定类型的信息





### 类型安全转换 - `dynamic_cast`

`dynamic_cast`运算符 是最常用的RTTI组件。

用途：

- 不能回答，指针指向的是哪类对象。
- 用于解决，**是否可以安全地将对象赋给特定类型的指针**。

  相比 “**指针指向的是哪种类型的对象**” 问题 **“类型转换是否安全”**  更通用、更有用。

  - 要调用方法，类型不一定要完全匹配，而可以是定义了方法的虚拟版本的基类类型。
  - 知道类型后，只需要知道调用特地的方法是否安全就足够了。

什么是安全的？

- 将**派生类对象**的地址，赋给**基类**的指针。**一定**是安全的。


- 将**基类对象**的地址，赋给**派生类**的指针。**不一定**是安全的，可能安全、也可能不安全。


```cpp
/* 类定义 */
class Grand {...}; // 包含虚方法
class Superb : public Grand {...};
class Magnificent : public Superb {...};
/* 对象 */
Grand  * pg = new Grand;
Superb * ps = new Superb;
Magnificent * pm = new Magnificent;
/* 类型转换 */
Magnificent * p1 = (Magnificent *) pm; // ok
Magnificent * p2 = (Magnificent *) pg; // 不一定ok
Grand * p3       = (Grand *) pm; // ok
```



语法

- `Super * pm = dynamic_cast<Superb *>(pg);`

  指针`pg`能否安全的转换为`Superb *`

  - 安全：返回对象的地址。

  - 不安全：返回`0`，即 空指针。

  ```cpp
  // 判断是否能安全的转换：
  /* 不能安全转换，则ps=0 空指针，不执行成员函数Say() */ 
  if (ps = dynamic_cast<Superb *>(pg)) 
      ps->Say();
  ```

- `Super & pm = dynamic_cast<Superb &>(pg);`

  也可以用于引用

  - 安全：返回对象的地址。

  - 不安全：

    - `bad_cast`类型的异常，表示请求不正确。

      `bad_cast`从`exception`类派生而来，在头文件`typeinfo`中定义。

    - 因为`0`表示空指针，但是没有与空指针对应的**引用值**。因此**无法使用特殊的引用值**表示失败。

    ```cpp
    #include <typeinfo> // for bad_cast
    
    try{
        
    }
    catch (bad_cast &){
        ...
    };
    ```



### typeid 运算符

`typeid`运算符：C++的关键字之一，等同于sizeof这类的操作符。在头文件typeinfo中定义。

> [C++ typeid关键字详解-CSDN博客](https://blog.csdn.net/gatieme/article/details/50947821)

- 头文件
  ```cpp
  // cpp  
  #include <typeinfo>
  // c
  #include <typeinfo.h>
  ```
  
- 返回值

  - 一个对`type_info`对象的引用，可用于获得关于对象的信息。
  - `bad_info`类的实现，随厂商而异，但都包含一个`name()`成员。

- 用途

  - 确定两个对象是否为同种类型。

    - `type_info`类重载了`==`和`!=`运算符
    - 可以使用这些运算符来对类型进行比较。

    - 对两个`typeid`的返回值进行比较，以确定对象是否为特定的类型，

  - 运行时获知变量类型名称。
    - 使用  `type_info`对象 的`name()方法`
    - `name()`方法，可以方便的将方法用于调试。
    - `name()`函数返回一个随实现而异的**字符串**，通常（不一定）是类的名称。

- 空指针异常

  - `bad_typeid`异常类型从`exception`类派生而来，在头文件`typeinfo`中声明。

```cpp
#include <typeinfo>
#include <typeinfo.h> // 或c语言风格

int ia = 3;
// 需要注意不是所有编译器都输出”int”、”float”等之类的名称，对于这类的编译器可以这样使用
if(typeid(ia) == typeid(int))
{
    cout <<"int" <<endl;
    // 运行时获知变量类型名称  typeid(变量).name()
    cout << typeid(ia).name() << endl;
}


// 空指针异常
Magnificent * pg = new Magnificent;
typeid(Magnificent) = typeid(*pg); // pg是空指针，则程序引发bad_typeid异常。
```





### 示例

`typeid`测试用来针对具体的类，选择一种操作。

> 操作不是类的方法，所以不能通过类指针调用。



```cpp
// rtti2.cpp  -- using dynamic_cast, typeid, and type_info
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <typeinfo>
using namespace std;

class Grand
{
private:
    int hold;
public:
    Grand(int h = 0) : hold(h) {}
    virtual void Speak() const { cout << "I am a grand class!\n";}
    virtual int Value() const { return hold; }
};

class Superb : public Grand
{
public:
    Superb(int h = 0) : Grand(h) {}
    void Speak() const {cout << "I am a superb class!!\n"; }
    virtual void Say() const
        { cout << "I hold the superb value of " << Value() << "!\n";}
};

class Magnificent : public Superb
{
private:
    char ch;
public:
    Magnificent(int h = 0, char cv = 'A') : Superb(h), ch(cv) {}
    void Speak() const {cout << "I am a magnificent class!!!\n";}
    void Say() const {cout << "I hold the character " << ch <<
               " and the integer "  << Value() << "!\n"; }
};

Grand * GetOne();
int main()
{
    srand(time(0));
    Grand * pg;
    Superb * ps;
    for (int i = 0; i < 5; i++)
    {
        pg = GetOne();
        cout << "Now processing type " << typeid(*pg).name() << ".\n";
        pg->Speak();
        if( ps = dynamic_cast<Superb *>(pg))
            ps->Say();
        if (typeid(Magnificent) == typeid(*pg))
            cout << "Yes, you're really magnificent.\n";
    }
    // std::cin.get();
    return 0;
}

Grand * GetOne()
{
    Grand * p;

    switch( rand() % 3)
    {
        case 0: p = new Grand(rand() % 100);
                    break;
        case 1: p = new Superb(rand() % 100);
                    break;
        case 2: p = new Magnificent(rand() % 100, 'A' + rand() % 26);
                    break;
    }
    return p; 
}
```





### 误用

尽量使用`dynamic_cast`和虚函数。

```cpp
/* 使用dynamic_cast */
Grand * pg;
Super * ps;
for (int i = 0; i < 5; i++)
{
    pg = GetOne();
    pg->Speak();
    if (ps = dynamic_cast<Superb *>(pg) )
    	pg->Say();
}
```

不推荐使用`typeid`运算符进行逐一判断，因为：

- 代码更长更难看。
- 显式地指定各个类存在严重的缺陷。

> 如果使用`typeid`显式地测试每个类型时，必须修改`for`循环的代码。
>
> 即：添加`else if`，但无需修改原来的版本。

```cpp
Grand * pg;
Super * ps;
Magnificent * pm;
for (int i = 0; i < 5; i++)
{
    pg = GetOne();
    if (typeid(Magnificent) == typeid(*pg))
    {
        pm = (Magnificent *) pg;
        pm->Speak();
        pm->Say();
    }
    if (typeid(Superb) == typeid(*pg))
    {
        pm = (Superb *) pg;
        pm->Speak();
        pm->Say();
    }
    else
        pg->Speak();
}
```





# 类型萃取(编译期) 

编译期获知变量的类型：

有时，对于同一个函数（或者算法），他们的操作是类似的，但是对于不同的类型，会有一些细微的改变，这个时候类型萃取就能比较简单地实现这个需求。

类型萃取不仅可以检查类型属于哪一种。还可以对类型进行修改，比如添加`const`，添加引用等

## integral_constant

### 引子: 模板参数

#### 类型 或 常量

在模板编程中，模板参数只能使用 类型 或 字面常量

```cpp
template<bool bool_val, typename T>
class A ...

A<true, int> aInt;   // 正确 字面常量
A<false, int> bInt;  // 正确 字面常量

bool bool_val_1 = false;
A<bool_val_1, int> aInt; // 编译出错, 需要声明 字面常量表达式 constexpr

constexpr bool bool_val_2 = true;
A<bool_val_2, int> aInt;   // 正确 字面常量表达式
```

1. `类型`
- `int`, `double`, `float`
     - 自定义类型class。
- 以及，后续会介绍的：字面常量的类型`std::integral_constant<typename _Tp, _Tp __v>::value_type`

2. 常量：

   > `<>`中除了类型，还可以要添加值（字面常量）。

     - 字面常量表达式 `constexpr bool cond = true;`

   - 字面常量：`"value"`，`1`，`'c'`，`true`...

#### 不便之处

需求：即想要  根据传入的类型去实例化对象，并且传入该类型的默认值。

解决：两个参数分别指明`类型` 和 `字面常量`。显式标注模板参数类型，偏特化。

- 两个参数，
  - 一个指定`类型`，
  - 一个指定`字面常量`
- 例如
  - `template<typename T, T X>` 
  - `func01<int, 1>()`   `func01<double, 1.0>()`

偏特化（显式固定的值）

- 实现处：需要提供偏特化——大量的冗余重复。
- 使用处：方便。

```cpp
// 要求
//  - 创建 T 类型的变量
//  - 使用 默认值 bool/int X

// 不显示指定默认值类型
template<typename T, T X>
void mFunc(){ }
// 特例：偏特化，显示地指定类型(优先使用)
template<typename T, bool X>
void mFunc(){ }

// 缺点: 虽然使用时无感，但是要支持其他通用的类型的常量，往往需要把同样的代码抄一遍
mFunc<double, 10.0>(); // 使用typename T, T X
mFunc<int, 10>();    // 使用偏特化
mFunc<bool, 10>();   // 不支持
```





#### 类型字面常量

用一个参数，同时表示 `类型` 和  `字面常量`。

3. 兼具两者的`integral_constant`，`类型字面常量`。

更好的办法，利用一个抽象同时把`类型`和`字面常量`（默认值）表示起来，可以同时允许 `创建 T 类型的变量` 以及 `使用 默认值` 。

使用`integral_constant`

- `template<typename T>` 
- `func01<std::integral_constant<bool, false>>();`具体见下文

从抽象上来说，代码的实现方直接把`类型字面常量`抽象成一个模板，并且`type/value`可以引用到相应的类型和值。实现处少写，使用处多写。

```cpp
// 答：配合integral_constant使字面量变成类型typename
template<typename constexpr_val>
void mFunc(){ }

// 字面量无法作为类型
mFunc<true>();
mFunc<false>();
// 使用配合integral_constant使字面量变成类型typename
// 详情见下文
typedef std::integral_constant<bool, false> false_type;
mFunc<false_type>();
```



### 原理

这个类是`type_traits`的基石，基本上`type_traits`都直接或间接继承自`integral_constant`。提供了

- 一个变量：静态编译期常量 `value`
  - 静态变量 要在类外声明；
  - 实际上，在c++11中constexpr应该就已经不需要重新声明定义了
- 两个 `typedef`
  - `value_type`，对`类型`取别名 
  -  `type`，对 `integral_constant` 这个`类型常量`本身取别名

- 两个 操作符重载
  - `value_type()`和`operator()()`
  - 均返回  静态编译期常量 `value`

```cpp
/// integral_constant
template<typename _Tp, _Tp __v>
struct integral_constant {
    // 注意static
    static constexpr _Tp                  value = __v;
    typedef _Tp                           value_type;
    typedef integral_constant<_Tp, __v>   type;
    constexpr operator value_type() const { return value; }
    constexpr value_type operator()() const { return value; }
};

// 再次声明: 类外声明 静态成员变量(静态变量 要在类外声明)
// 实际上，在c++11中constexpr应该就已经不需要重新声明定义了
template<typename _Tp, _Tp __v>
constexpr _Tp integral_constant<_Tp, __v>::value;
```

```cpp
struct mStruct {
    // 注意static
    typedef int                           value_type;
};

int main() {
    mStruct::value_type mInt = 10;
```



### typedef

使用 `std::integral_constant` 对 "类型的常量" 进行替换。

```cpp
// 重命名 std::integral_constant
typedef std::integral_constant<bool, false> false_type;

// SomeCode<int, false> aInst; 被替换为
func02<false_type> aInst;
// 即:
func02<std::integral_constant<bool, false>> aInst;
```

```cpp
// 重命名 std::integral_constant
typedef std::integral_constant<int, 10> bint10_type;

// SomeCode<int, 10> bInst; 被替换为
func02<bint10_type> bInst;
// 即:
func02<std::integral_constant<int,10>> bInst;
```

一些integral_constant示例：

```cpp
/// The type used as a compile-time boolean with true/false value.
typedef integral_constant<bool, true>     true_type;
typedef integral_constant<bool, false>    false_type;
// __bool_constant这是一个辅助类型重定义，可以用在模板函数重载中
template<bool __v>
using __bool_constant = integral_constant<bool, __v>;
```

`type_traits`中，大部分类都直接或间接继承自`integral_constant`，

- 凡是带前缀`is_*`的类的**偏特化**或**特化**基本都是继承自`true_type`。
- 例如 `__is_pointer_helper` ， `_Tp*`匹配失败就会继承`false_type`，调用操作符或 `::value`得到的自然就是 false，反之，成功匹配就返回 true。

```cpp
template<typename>
struct __is_pointer_helper : public false_type { };

template<typename _Tp>
struct __is_pointer_helper<_Tp*> : public true_type { };
```



### 使用

客户端（使用函数的地方）：使用方式上就会有点麻烦了。

- 获取 `类型`——变量 `::value_type`
- 获取 `静态编译期常量`——变量`::value`，操作符重载`value_type()`  ，操作符重载 `operator()()`
- 获取 `类型常量`  `integral_constant` 本身—— 变量 `::type`

```cpp
template<typename integral>
void func02(T param) {
    // 使用 类型常量integral_constant
    typedef typename integral::type type;
    // 使用 类型
    typedef typename integral::value_type value_type;
    value_type mVal = 10;
    // 使用 字面常量
    value_type mVal2 = integral::value;
    
};
```





## 获取类型

### `decay<>` 还原

去除cv限定符，引用，etc。

#### 用途

`std::decay`就是对一个类型进行退化处理：把各种引用，cosnt，volitale，去掉。这样就能通过`std::is_same`正确识别出加了引用的类型了

- 在模板里可以通过std::is_same判断模板的类型，从而实现对不同类型的区别对待
- 在对类型要求不是非常严格的情况下，可以使用std::decay把类型退化为基本形态，结合std::is_same用，可以判断出更多的情况

#### 应用

涉及到模板类型时，需要 typename关键字（加了肯定没错，不加可能有错）

- `typename decay<decltype(xx)>::type`
- `typename decay<T::value_type>::type`

cpp17，支持decay_t。

- 应用：`decay_t<T::value_type>`  `decay_t<decltype(xx)>`

- 这种 `_t` 基本就是对 `integral_constant::type` 成员 取别名，

- 它做的事就是 `using decay_t = typename std::decay<decltype(t)>::type `。

  `std::decay_t<decltype(t)>` 等价于  `typename std::decay<decltype(t)>::type`

  ```cpp
  template<typename _Tp>
  using decay_t = typename decay<_Tp>::type;
  ```

#### 实现

```cpp
template< class T >
struct decay {
private:
    typedef typename std::remove_reference<T>::type U;
public:
    typedef typename std::conditional< 
        std::is_array<U>::value,
        typename std::remove_extent<U>::type*,
        typename std::conditional< 
            std::is_function<U>::value,
            typename std::add_pointer<U>::type,
            typename std::remove_cv<U>::type
        >::type
    >::type type;
};
```



### decltype

#### 用途

**decltype用途**：编译期获知 变量/表达式 的类型

- 编译期用于获得与某一 `变量` 或 `表达式`的类型
- 从而 声明/声明变量 或 进行类型萃取

` decltype(变量名)` 和 `decltype(表达式)`

- 表达式

  - 不执行：仅仅获得表达式的类型

    例如：函数不被调用，仅仅获得返回值类型

- 变量

  - 若希望得到引用类型  例如`int &`，同样通过 `decltype((变量名))`

    强制编译器将变量视为表达式，从而得到引用类型

  - 左值引用，是一定需要初始化的。



#### 类型推导4规则

`decltype(expression)`

- 没有带括号的标记符表达式 / 类成员访问表达式

  expression 所命名的实体的类型。

  - 包括const等cv限定符，以及引用&。


`decltype((expression))`

- 表示添加引用。注：左值引用类型是一定要初始化的。

`decltype(被重载的函数)`

- 则会导致编译错误。

变量：假设变量e的类型是T

| 变量e的类型 | 表达式          | 类型  |
| ----------- | --------------- | ----- |
| `将亡值`    | `decltype(e)`   | `T&&` |
| `左值`      | `decltype(e)`   | `T&`  |
| `右值`      | `decltype(e)`   | `T`   |
|             | `decltype((e))` | `T&`  |

#### 从表达式推导

##### decltype(auto) 万能推导

来自变量 / 返回值

decltype(auto)用来声明变量以及指示函数返回类型。

被用于声明变量时，该变量必须立即初始化。

在推导函数返回值类型时，

- 先用返回值表达式替换decltype(auto)当中的auto，
- 再根据decltype的语法规则来确定函数返回值的类型。

```cpp
template<typename T>  
T f();
 
struct S {int a;};
 
int a = 0;
decltype(auto) g1() {return s.a;}
decltype(auto) g2() {return std::move(a);}
decltype(auto) g3() {return (a);}
decltype(auto) g4() {return (0);}
 
int main()
{
    int a = 0;
    decltype(auto) i1 = a;
    decltype(auto) i2 = std::move(a);
    decltype(auto) i3 = (s.a);
    decltype(auto) i4 = (0);
    f<decltype(i1)>();
    f<decltype(i2)>();
    f<decltype(i3)>();
    f<decltype(i4)>();
    f<decltype(g1())>();
    f<decltype(g2())>();
    f<decltype(g3())>();
    f<decltype(g4())>();
}
```





##### declval 从类型到值

最终目的仍然是decltype获得类型

- `decltype(declval<F>()(参数们))` 

  配合declval获得一个值，再根据这个值进一步推导类型。

- 编译器凭空创建一个函数对象（尽管其可能并没有构造函数）
- declval只能用在不求值的表达式（编译期的语法糖）



#### 取别名typedef

使用typedef取别名：

- 未来需要多次使用该类型。
- 甚至可以使用匿名结构体。





### integral_constant

对于integral_constant来说，获取其中封装的类型 `::value_type`

```cpp
// 普通函数体中
typedef std::integral_constant<int, 10> int_10;
typedef typename int_10::value_type value_type; // int
value_type mVal = 10;

// 在模板函数中
// 加了typename一定没错，不加typename可能有错。
template<typename T>
void function(
	typename T::value_type val_1,
	// 或者
	T::value_type val_2) {}
```

模板函数中的具体规则：如果包含间接包含模板参数时，需要加typename。

因为编译期对这玩意做不到区分

- 到底是某个类里的 名字为type的成员变量 typedef 函数指针...
- 还是 type_trait声明的类型的指针，

所以需要typename进行特殊化。



### 模板函数返回值

invoke_result_t & declval

只知道函数的模板类型，没有函数对象。希望得到其返回值类型。

要求：函数存在没有参数的重载。

- `std::invoke_result_t<F>` 

  - 表示空调用后的返回类型
  - 缺点：参数列表可能不为空

- `decltype(f(1, 2, 3))`

  - 缺点：可能不清楚合法的参数列表

- `decltype(declval<F>()())` 

  - decltype 配合 declval

  - 编译器凭空创建一个函数对象（尽管其可能并没有构造函数）

    `declval` 只能用在不求值的表达式（编译期的语法糖）



### common_type

返回参数列表中的参数都可以转换成的类型（有些拗口）。

```cpp
// 返回值为double
typedef std::common_type<int, float, double>::type testType1;
std::is_same<double, testType1>::value; // true

typedef std::common_type<char,std::string>::type testType2;      // 报错，无法互相转换
```

可能的实现：





## 判断类型

### 编译器提供

#### is_same

小技巧：忽略if分支

`constexpr`解决编译期出错 `std::is_same_v`   会在编译器略过错误分支，跳过else的非法命令 解决编译失败。

##### ::value

不同于，这里的value就是类型是否一样。

在模板里可以通过std::is_same判断模板的类型，从而实现对不同类型的区别对待

##### is_same_v

`std::is_same_v` 判断类型是否相同。默认是decay的。

否则 会认为const,   &,  &&, 数组和指针不同，例如 vector的`operator[]()`返回的就是引用。需要使用`std::decay_t<decltype(t)>`。

```cpp
int main() {
    using namespace std;
    cout << is_integral<int>::value << endl; //int属于整型，所以输出1
    cout << is_integral<double>::value << endl; //double不属于整型，所以输出0
    cout << is_integral_v<int> << endl; //这是更好的写法，输出1
    cout << is_integral_v<double> << endl; //这是更好的写法，输出0
}
```



#### is_convertible

用于检查是否可以将任何数据类型A隐式转换为任何数据类型B。返回布尔值true或false。

```cpp
template< class From, class To >
struct is_convertible;

template< class From, class To >
struct is_nothrow_convertible;


is_convertible <A*, B*>::value;
// 采用A和B两种数据类型
// - A  代表要转换的参数。
// - B  代表参数A隐式转换的参数。
// 代表 A 能否隐式转换到 B
// 不确定，同时代表 B 能否显式转换到 A?
```

is_convertible 继承自integral_constant，

- 如果能转换， `is_convertible<>::value` 萃取到 true；
- 如果不能转换，萃取到false。

```cpp

// Derived子类转Base基类
bool BtoA = is_convertible<Derived*, Base*>::value; // true
bool InttoConst = is_convertible<int, const int>::value;	// true
```



### is_xx

`std::is_integral_v` 是否是int类型。还有很多很多别的类模板，和`is_integral`类似

#### 实现

`std::is_xxx`的实现基于模板特化

- 当参数类型为`xxx`时
  - `std::is_xxx`模板被特化，
  - 其`value`成员被设为`true`；
- 否则，继承自`std::integral_constant`
  - 其`value`成员为`false`。

下面列举一些可能的实现。

```cpp
template<class T>
struct is_fundamental
    : std::integral_constant<
        bool,
        std::is_arithmetic<T>::value ||
        std::is_void<T>::value ||
        std::is_same<std::nullptr_t, typename std::remove_cv<T>::type>::value
        // you can also use 'std::is_null_pointer<T>::value' instead in C++14
> {};
```

```cpp
// Note: this implementation uses C++20 facilities
template<class T>
struct is_integral : std::bool_constant<
    requires (T t, T* p, void (*f)(T)) // T* parameter excludes reference types
    {
        reinterpret_cast<T>(t); // Exclude class types
        f(0); // Exclude enumeration types
        p + t; // Exclude everything not yet excluded but integral types
    }> {};
```

```cpp
template<class T>
struct is_floating_point
     : std::integral_constant<
         bool,
         // Note: standard floating-point types
         std::is_same<float, typename std::remove_cv<T>::type>::value
         || std::is_same<double, typename std::remove_cv<T>::type>::value
         || std::is_same<long double, typename std::remove_cv<T>::type>::value
         // Note: extended floating-point types (C++23, if supported)
         || std::is_same<std::float16_t, typename std::remove_cv<T>::type>::value
         || std::is_same<std::float32_t, typename std::remove_cv<T>::type>::value
         || std::is_same<std::float64_t, typename std::remove_cv<T>::type>::value
         || std::is_same<std::float128_t, typename std::remove_cv<T>::type>::value
         || std::is_same<std::bfloat16_t, typename std::remove_cv<T>::type>::value
     > {};
```

`std::is_enum` 的实现通常依赖于编译器提供的内部特性来判断类型是否为枚举类型。



#### is_xx_v

在C++17及以后的版本中，同样可以使用带有`_v`后缀的变量模板。

`std::is_xxx<mtype>::value`等价于`std::is_xxx_v<mtype>`

这个类型特征的实现同样基于模板特化。仅当参数类型为`std::nullptr_t`时，`std::is_null_pointer`的`value`成员被特化为`true`。

```cpp
std::is_void<void>::value       // true
std::is_void<int>::value        // false


std::is_void_v<void>          // true
std::is_void_v<int>           // false
```

表格概述了C++中的基本类型类别（Primary Type Categories）以及对应的类型特征，包括它们在`<type_traits>`头文件中的定义和各自的作用：

| 类型特征                        | C++版本 | 描述                                   |
| ------------------------------- | ------- | -------------------------------------- |
| std::is_void                    | C++11   | 检查类型是否为void                     |
| std::is_null_pointer            | C++14   | 检查类型是否为std::nullptr_t           |
| std::is_integral                | C++11   | 检查类型是否为整型                     |
| std::is_floating_point          | C++11   | 检查类型是否为浮点型                   |
| std::is_array                   | C++11   | 检查类型是否为数组类型                 |
| std::is_enum                    | C++11   | 检查类型是否为枚举类型                 |
| std::is_union                   | C++11   | 检查类型是否为联合体                   |
| std::is_class                   | C++11   | 检查类型是否为非联合类类型             |
| std::is_function                | C++11   | 检查类型是否为函数类型                 |
| std::is_pointer                 | C++11   | 检查类型是否为指针类型                 |
| std::is_lvalue_reference        | C++11   | 检查类型是否为左值引用                 |
| std::is_rvalue_reference        | C++11   | 检查类型是否为右值引用                 |
| std::is_member_object_pointer   | C++11   | 检查类型是否为指向非静态成员对象的指针 |
| std::is_member_function_pointer | C++11   | 检查类型是否为指向非静态成员函数的指针 |

这个表格提供了一个简洁的总览，可以帮助理解和使用C++中的这些基本类型特征。这些特征是模板元编程中的重要工具，它们允许在编译时进行类型检查和推导，从而使得代码更加通用、灵活且类型安全。



#### 子集关系

类型检查的过程属于编译时计算，不会增加运行时的开销。

通过编译时模板特化和SFINAE（Substitution Failure Is Not An Error，替换失败不是错误）技术实现的。SFINAE的原理是，在模板实例化过程中，如果某个替换导致编译错误，这个错误会被忽略，编译器会寻找其他可能的模板特化或重载。

这些子集关系在 C++ 的类型特征中非常重要，特别是在模板元编程和类型约束方面，它们帮助确保类型的正确使用和函数的适当重载。

**基础类型**

基础类型 `std::is_fundamental` 包括

- 算术类型 `std::is_arithmetic`：专指涉及数值计算的类型

  > 在编写一个只接受数值类型参数的模板函数时，可以使用`std::is_arithmetic`来静态断言传入的类型符合要求

  - 整数类型  `std::is_integral`，int, char, long 等
  - 浮点类型  `std::is_floating_point`，float, double, long double等

- `void` 类型 `std::is_void`

**复合类型** `std::is_compound` 包括

> 基本类型的反义词，复合类型是由基本类型构造出来的类型。在C++中，任何类型要么是基本类型，要么是复合类型。

- 类类型 `std::is_class`
- 枚举类型  `std::is_enum`
- 联合类型 `std::is_union`
- 数组类型 `std::is_array`
- 函数类型 `std::is_functio`
- 指针类型 `std::is_pointer`

**标量类型**`std::is_scalar`包括

- 算术类型 `std::is_arithmetic`
- 枚举类型 `std::is_enum`
- 指针类型 `std::is_pointer`
- 成员指针 `std::is_member_pointer`
- 空指针`nullptr_t` `std::is_null_pointer`

**对象类型**

对象类型 `std::is_object`，包括

> 在C++中是指拥有固定大小和存储的任何非函数类型
>
> 类型特征被用来判断一个类型是否为对象类型。

- 标量类型、
- 数组、
- 类类型等。



