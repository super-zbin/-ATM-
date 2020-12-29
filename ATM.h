#ifndef ATM_H
#define ATM_H
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
//1、问题描述：
//模拟银行的自动取款机使用过程中的界面和用户交互过程。设计出主界面，实现查询银行卡余额、取款、修改密码、退出系统等功能。这一系列功能要在后台中显示出来。
//2．功能要求：
//	卡号、密码输入最多三次，否则直接退出系统。
//	取款功能：取款金额受卡余额、机单笔最大取款金额及机当前剩余金额的限制。
//	查询功能：实现查询余额功能。
//	更改密码：实现密码更改功能，对于密码要有2次验证。
//	锁卡功能：实现卡被锁的功能。
//	退卡功能：实现退出系统功能。
//	保存功能：可将当前系统中各类记录存入文件中，存入方式任意。
//	读取功能：可将保存在文件中的信息读入到当前系统中，供用户进行使用。

typedef struct Card
{
	char account[13];		// 卡号
	char password[7];	// 密码
	bool isLocked;		// 标记卡片是否被锁
	int balance;		// 卡内余额
} Card;

class ATM
{
public:
    ATM();
    virtual ~ATM();
    void run();     //主要运行
    void display(); //主界面
    void login();   //登录界面
    bool idExist(const string &);       //检验银行卡账号是否存在
    bool passwordExist(const string &);//验证银行卡密码
    void checkBalance();//查询余额
    void drawMoney();   //取款
    void changePass();  //修改密码
    void exitSystem();  //退出系统

protected:

private:
    Card curCard;
    int cardInd;//存储当前银行卡在配置文件的索引
    unsigned int limit;//单笔最大取款金额
    unsigned int surplus;//剩余金额
    ifstream ifsCard;
    ofstream ofsCard;
};

#endif // ATM_H
