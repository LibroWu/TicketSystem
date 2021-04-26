# TicketSystem:LaMétropole

## 模块划分图



## 各模块功能



## 类设计

#### namespace LaMetropole

以下所有的类都包含在LaMetropole的名字空间内，以避免与std内的重名。

### paser相关类

* **paser**

  用于接受命令行，返回处理后的信息。

  包含在paser类内的子类:

  * **paser::PaperCup**

    用于保存处理后的信息

    包含成员变量：

    * `char arv`参数总数 
    * `string* arg[26]`  `-(i+'a')`对应的指令
    * `char key[26]`各参数对应的指示符

  * **paser::tokenScanner**

    用于将一个字符串按给定的分割符分割。

    包含成员函数：

    * `bool hasNextToken()`
    * `string* nextToken()`

  成员函数：

  * `PaperCup* listen()`
    * 读取一行命令，处理后以`paser::PaperCup`的格式返回

### user相关类

* **user**

  用于储存用户的信息

  * 包含成员变量
    * `char username[]`
    * `char password[]`
    * `char priviege`
    * `char name[]`
    * `char mailAddr[]`
  * 
    
  * 构造函数

* **userManager**

  * 处理与用户有关的操作
  * 包含成员变量
    
* 用于储存用户信息及提供检索的BPT
    * 用于维护用户登录态的unordered_map
  
  * 包含成员函数

    * `void addUser(LaMetropole::paser::PaperCup*)`

      增加一名用户

    * `void login(LaMetropole::paser::PaperCup*)`

      登录一名用户

    * `void logout(LaMetropole::paser::PaperCup*)`

      登出一名用户

    * `void queryProfile(LaMetropole::paser::PaperCup*)`

      查询一名用户的信息

    * `void modifyProfile(LaMetropole::paser::PaperCup*)`
  
      修改一名用户的信息

### Train相关类

* **trainManager**

  * 处理与火车相关的操作

  * 包含成员变量

    * 用于储存

  * 包含成员函数

    * `void addTrain(LaMetropole::paser::PaperCup*)`

      增加一辆火车

    * `void releaseTrain(LaMetropole::paser::PaperCup*)`

      释放一辆火车

    * `void queryTrain(LaMetropole::paser::PaperCup*)`

      询问符合条件的火车

    * `void deleteTrain(LaMetropole::paser::PaperCup*)`

      删除一辆火车

    * `void queryTransfer(LaMetropole::paser::PaperCup*)`

      询问符合条件的换乘路线

    * `void queryTicket(LaMetropole::paser::PaperCup*)`

      询问票

    * `void buyTicket(LaMetropole::paser::PaperCup*)`

      买票

    * `void queryOrder(LaMetropole::paser::PaperCup*)`

      询问订单

    * `void refundTicket(LaMetropole::paser::PaperCup*)`

      

    * `void Clean(LaMetropole::paser::PaperCup*)`

    * `void Exit(LaMetropole::paser::PaperCup*)`

### 数据库相关类

* **MemoryRiver**

  用于文件读写的类，支持空快的回收

* **BPT**

  不支持重复key，使用`MemoryRiver`类的BPlusTree

  * 实例化参数`template<class T,class U,int M,int L,class Compare=std::less<T>`：
    * T为作为Key值的变量
    * U为储存的变量
    * M为非叶节点的大小
    * L为叶节点的大小
    * Compare为比较函数

### 映射相关类

* **unordered_map**

  模仿std::unordered_map，含有operator[]等函数（不含迭代器）

* **map**

  模仿std::map，用红黑树实现

### 其他功能类

* pair

  模仿std::pair

## 文件设计



## 组员工作情况

