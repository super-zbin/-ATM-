#include "ATM.h"

ATM::ATM()
{
    ifsCard.open("Cards.dat",std::ios::in | std::ios::binary);//保证ATM机运行时能够对配置文件进行读取
    //读取ATM机信息：机单笔最大取款金额及机当前剩余金额的限制
    ifstream ifs("ATM.dat",std::ios::in | std::ios::binary);
    ifs.read((char *)&limit,sizeof(int));
    ifs.read((char *)&surplus,sizeof(int));
    ifs.close();
}

ATM::~ATM()
{
    //记录ATM机信息：机单笔最大取款金额及机当前剩余金额的限制
    ofstream ofs("ATM.dat",std::ios::out | std::ios::binary);
    ofs.write((char *)&limit,sizeof(int));
    ofs.write((char *)&surplus,sizeof(int));
    ofs.close();
    //关闭对配置文件的读取
    if(ifsCard.is_open())
        ifsCard.close();
}
bool ATM::idExist(const string &ID)
{
    int cnt = 0;
    bool flag = 0;
    char tmpID[13];
	ifsCard.read((char*)&cnt, sizeof(int));
	for (int i = 0; i < cnt; i++)
	{
		ifsCard.read(tmpID,13);
		if (!ID.compare(tmpID))  // string::compare 标准库内置函数，相同则返回0，不同则大于或小于0
		{
			flag = true;
			cardInd = i;
			break;
		}
		ifsCard.seekg(sizeof(Card)-13,std::ios_base::cur);
	}
	return flag;
}
bool ATM::passwordExist(const string &PASSWORD)
{
    bool flag = 0;
    char tmpPassword[7];
    ifsCard.seekg(sizeof(Card)*cardInd + 4 + 13,std::ios_base::beg);
    ifsCard.read(tmpPassword,7);
    if(!PASSWORD.compare(tmpPassword))
    {
        flag = true;
    }
    return flag;
}
void ATM::login()
{
    char id[13],password[7];
    int cnt_id,cnt_password;
    cnt_id = 0;cnt_password = 0;//记录卡号和密码的输入次数
    cout<<"*****ATM机就绪*****"<<endl;
    while(1)
    {
        if(cnt_id == 3)
        {
            cout<<"卡号输入超过3次错误，退出系统"<<endl;
            exit(1);
        }
        //1.输入银行卡账号
        cout<<"请输入银行卡账号：";
        cin>>id;
        //在Cards.dat文件查询是否有该账号，若有则继续输入密码，无则输出错误并跳出该次循环
        if(idExist(id))
        {
            cout<<"请输入相应的密码：";
            while(1)
            {
                cin>>password;
                if(passwordExist(password))
                {
                    ifsCard.seekg(sizeof(Card)*cardInd + 4,std::ios_base::beg);
                    ifsCard.read((char *)&curCard,sizeof(Card));
                    cout<<curCard.account<<" "<<curCard.password<<" "<<curCard.isLocked<<" "<<curCard.balance<<endl;
                    cout<<"*****成功登陆*****"<<endl;
                    return;
                }
                else
                {
                    cnt_password++;
                    cout<<"银行卡密码输入错误，请重新输入"<<endl;
                    continue;
                }
            }
        }
        else
        {
            cnt_id++;
            cout<<"银行卡账号输入错误，请重新输入"<<endl;
            continue;
        }
    }
}
void ATM::display()
{
    cout<<"*******************************"<<endl;
    //查询银行卡余额、取款、修改密码、退出系统
    cout<<"1.查询银行卡余额"<<endl;
    cout<<"2.取款"<<endl;
    cout<<"3.修改密码"<<endl;
    cout<<"4.退出系统"<<endl;
}
void ATM::checkBalance()
{
    cout<<"当前余额："<<curCard.balance<<endl;
    return;
}
//取款功能：取款金额受卡余额、机单笔最大取款金额及机当前剩余金额的限制。
void ATM::drawMoney()
{
    unsigned money;
    cout<<"ATM机当前余额："<<surplus<<endl;
    cout<<"单笔最大取款金额："<<limit<<endl;
    cout<<"用户当前余额："<<curCard.balance<<endl;
    cout<<"请输入取款金额：";
    while(1)
    {
        cin>>money;
        if(money < 0)
        {
            cout<<"输入错误，请重新输入:";
            continue;
        }
        else if(money <= surplus)
        {
            if(money <= limit)
            {
                if(money <= curCard.balance)
                {
                    surplus -= money;
                    curCard.balance -= money;
                    cout<<"ATM机当前余额："<<surplus<<endl;
                    cout<<"用户当前余额："<<curCard.balance<<endl;
                    return;
                }
                else if(money > curCard.balance)
                {
                    cout<<"当前用户余额不足，请重新输入:";
                }
            }
            else if(money > limit)
            {
                cout<<"所取金额超过单笔最大取款金额，请重新输入:";
                continue;
            }
        }
        else if(money > surplus)
        {
            cout<<"所取金额超过ATM机余额，请重新输入:";
            continue;
        }
    }
}
//更改密码：实现密码更改功能，对于密码要有2次验证。
void ATM::changePass()
{
    char tmp[10];
    int i;
    cout<<"请输入当前密码：";
    while(1)
    {
        cin>>tmp;
        if(!strcmp(curCard.password,tmp))
        {
            cout<<"请再次输入密码：";
            cin>>tmp;
            if(!strcmp(curCard.password,tmp))//两次判断输入的密码
            {
                cout<<"密码输入正确"<<endl;
                /* 输入新密码 */
                while(1)
                {
                    cout<<"请输入新密码：";
                    cin>>tmp;
                    if(strlen(tmp) > 6)
                    {
                        cout<<"密码格式不正确，请重新输入新密码：";
                        continue;
                    }
                    /* 判断新密码的格式 */
                    for(i = 0;i < 6;i++)
                    {
                        if(tmp[i] <= '9' && tmp[i] >= '0')
                            continue;
                        else
                            break;
                    }
                    if(i == 6)
                    {
                        strcpy(curCard.password,tmp);
                        cout<<"新密码："<<curCard.password<<endl;
                        return;
                    }
                    else
                    {
                        cout<<"密码格式不正确，请重新输入新密码：";
                        continue;
                    }
                }
            }
            else
            {
                cout<<"密码输入错误，重新开始输入密码：";
                continue;
            }
        }
        else
        {
            cout<<"密码输入错误，请重新输入：";
            continue;
        }
    }

}
void ATM::exitSystem()
{
    if(ifsCard.is_open())
    {
        ifsCard.close();
    }
    //以读写模式打开，能够随机读写文件，将当前银行卡信息存储到配置文件中
    ofsCard.open("Cards.dat",std::ios::in | std::ios::out | std::ios::binary );
	ofsCard.seekp(sizeof(int) + cardInd* sizeof(Card),std::ios_base::beg);
	ofsCard.write((char*)&curCard, sizeof(Card));
    ofsCard.close();
}
void ATM::run()
{
    unsigned short choice;
    //1.输入银行卡账号和密码
    login();
    while(1)
    {
        display();
        //2.选择所需功能
        cin>>choice;
        switch(choice)
        {
            //1、查询余额
            case 1:checkBalance();break;
            //2、取款
            case 2:drawMoney();break;
            //3、修改密码
            case 3:changePass();break;
            //4、退出系统
            case 4:exitSystem();return;break;
        }
    }
}
