#include "ATM.h"

ATM::ATM()
{
    ifsCard.open("Cards.dat",std::ios::in | std::ios::binary);//��֤ATM������ʱ�ܹ��������ļ����ж�ȡ
    //��ȡATM����Ϣ�����������ȡ�������ǰʣ���������
    ifstream ifs("ATM.dat",std::ios::in | std::ios::binary);
    ifs.read((char *)&limit,sizeof(int));
    ifs.read((char *)&surplus,sizeof(int));
    ifs.close();
}

ATM::~ATM()
{
    //��¼ATM����Ϣ�����������ȡ�������ǰʣ���������
    ofstream ofs("ATM.dat",std::ios::out | std::ios::binary);
    ofs.write((char *)&limit,sizeof(int));
    ofs.write((char *)&surplus,sizeof(int));
    ofs.close();
    //�رն������ļ��Ķ�ȡ
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
		if (!ID.compare(tmpID))  // string::compare ��׼�����ú�������ͬ�򷵻�0����ͬ����ڻ�С��0
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
    cnt_id = 0;cnt_password = 0;//��¼���ź�������������
    cout<<"*****ATM������*****"<<endl;
    while(1)
    {
        if(cnt_id == 3)
        {
            cout<<"�������볬��3�δ����˳�ϵͳ"<<endl;
            exit(1);
        }
        //1.�������п��˺�
        cout<<"���������п��˺ţ�";
        cin>>id;
        //��Cards.dat�ļ���ѯ�Ƿ��и��˺ţ�����������������룬����������������ô�ѭ��
        if(idExist(id))
        {
            cout<<"��������Ӧ�����룺";
            while(1)
            {
                cin>>password;
                if(passwordExist(password))
                {
                    ifsCard.seekg(sizeof(Card)*cardInd + 4,std::ios_base::beg);
                    ifsCard.read((char *)&curCard,sizeof(Card));
                    cout<<curCard.account<<" "<<curCard.password<<" "<<curCard.isLocked<<" "<<curCard.balance<<endl;
                    cout<<"*****�ɹ���½*****"<<endl;
                    return;
                }
                else
                {
                    cnt_password++;
                    cout<<"���п����������������������"<<endl;
                    continue;
                }
            }
        }
        else
        {
            cnt_id++;
            cout<<"���п��˺������������������"<<endl;
            continue;
        }
    }
}
void ATM::display()
{
    cout<<"*******************************"<<endl;
    //��ѯ���п���ȡ��޸����롢�˳�ϵͳ
    cout<<"1.��ѯ���п����"<<endl;
    cout<<"2.ȡ��"<<endl;
    cout<<"3.�޸�����"<<endl;
    cout<<"4.�˳�ϵͳ"<<endl;
}
void ATM::checkBalance()
{
    cout<<"��ǰ��"<<curCard.balance<<endl;
    return;
}
//ȡ��ܣ�ȡ�����ܿ������������ȡ�������ǰʣ��������ơ�
void ATM::drawMoney()
{
    unsigned money;
    cout<<"ATM����ǰ��"<<surplus<<endl;
    cout<<"�������ȡ���"<<limit<<endl;
    cout<<"�û���ǰ��"<<curCard.balance<<endl;
    cout<<"������ȡ���";
    while(1)
    {
        cin>>money;
        if(money < 0)
        {
            cout<<"�����������������:";
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
                    cout<<"ATM����ǰ��"<<surplus<<endl;
                    cout<<"�û���ǰ��"<<curCard.balance<<endl;
                    return;
                }
                else if(money > curCard.balance)
                {
                    cout<<"��ǰ�û����㣬����������:";
                }
            }
            else if(money > limit)
            {
                cout<<"��ȡ�����������ȡ�������������:";
                continue;
            }
        }
        else if(money > surplus)
        {
            cout<<"��ȡ����ATM��������������:";
            continue;
        }
    }
}
//�������룺ʵ��������Ĺ��ܣ���������Ҫ��2����֤��
void ATM::changePass()
{
    char tmp[10];
    int i;
    cout<<"�����뵱ǰ���룺";
    while(1)
    {
        cin>>tmp;
        if(!strcmp(curCard.password,tmp))
        {
            cout<<"���ٴ��������룺";
            cin>>tmp;
            if(!strcmp(curCard.password,tmp))//�����ж����������
            {
                cout<<"����������ȷ"<<endl;
                /* ���������� */
                while(1)
                {
                    cout<<"�����������룺";
                    cin>>tmp;
                    if(strlen(tmp) > 6)
                    {
                        cout<<"�����ʽ����ȷ�����������������룺";
                        continue;
                    }
                    /* �ж�������ĸ�ʽ */
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
                        cout<<"�����룺"<<curCard.password<<endl;
                        return;
                    }
                    else
                    {
                        cout<<"�����ʽ����ȷ�����������������룺";
                        continue;
                    }
                }
            }
            else
            {
                cout<<"��������������¿�ʼ�������룺";
                continue;
            }
        }
        else
        {
            cout<<"��������������������룺";
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
    //�Զ�дģʽ�򿪣��ܹ������д�ļ�������ǰ���п���Ϣ�洢�������ļ���
    ofsCard.open("Cards.dat",std::ios::in | std::ios::out | std::ios::binary );
	ofsCard.seekp(sizeof(int) + cardInd* sizeof(Card),std::ios_base::beg);
	ofsCard.write((char*)&curCard, sizeof(Card));
    ofsCard.close();
}
void ATM::run()
{
    unsigned short choice;
    //1.�������п��˺ź�����
    login();
    while(1)
    {
        display();
        //2.ѡ�����蹦��
        cin>>choice;
        switch(choice)
        {
            //1����ѯ���
            case 1:checkBalance();break;
            //2��ȡ��
            case 2:drawMoney();break;
            //3���޸�����
            case 3:changePass();break;
            //4���˳�ϵͳ
            case 4:exitSystem();return;break;
        }
    }
}
