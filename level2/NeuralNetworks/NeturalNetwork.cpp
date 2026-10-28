#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <Windows.h>
#include "../../safe_input.h"
using namespace std;
const int side=8;
const int input_size=64;
const int hidden_size=64;
const int output_size=10;
const double learn_rate=0.1;
const double leaky_relu=0.01;
const int epochs=12;
const int train_capcity=140000;
const int test_capcity=10000;
double train_image[train_capcity][input_size];
unsigned char train_index[train_capcity];
double test_image[test_capcity][input_size];
unsigned char test_index[test_capcity];
struct Network
{
    double weighttohidden[input_size][hidden_size];
    double biashidden[hidden_size];
    double weighttooutput[hidden_size][output_size];
    double biasoutput[output_size];
    double hiddenpreactivation[hidden_size];
    double hiddenvalue[hidden_size];
    double outputpreactivation[output_size];
    double outputprobability[output_size];
};
string programdirectory;
void InitProgramDirectory()
{
    char buff[MAX_PATH];
    DWORD length=GetModuleFileNameA(NULL,buff,MAX_PATH);
    if (length==0||length>=MAX_PATH)
    {
        programdirectory="./";
        return;
    }
    string path(buff,length);
    size_t last=path.find_last_of("\\/");
    if (last!=string::npos)
        programdirectory=path.substr(0,last+1);
    else
        programdirectory="./";
}
double Activate(double x)
{
    return (x>0.0)?x:leaky_relu*x;
}
double Activatek(double x)
{
    return (x>0.0)?1.0:leaky_relu;
}
string Datapath(const string &path)
{
    return programdirectory+path;
}
void Forward(Network& net,const double input[input_size])
{
    for (int j=0;j<hidden_size;j++)
    {
        net.hiddenpreactivation[j]=net.biashidden[j];
        for (int i=0;i<input_size;i++)
            net.hiddenpreactivation[j]+=input[i]*net.weighttohidden[i][j];
        net.hiddenvalue[j]=Activate(net.hiddenpreactivation[j]);
    }
    for (int k=0;k<output_size;k++)
    {
        net.outputpreactivation[k]=net.biasoutput[k];
        for (int j=0;j<hidden_size;j++)
            net.outputpreactivation[k]+=net.hiddenvalue[j]*net.weighttooutput[j][k];
    }
    double maxvalue=net.outputpreactivation[0];
    for (int k=1;k<output_size;k++)
    {
        if (net.outputpreactivation[k]>maxvalue)
            maxvalue=net.outputpreactivation[k];
    }
    double esum=0.0;
    for (int k=0;k<output_size;k++)
    {
        net.outputprobability[k]=exp(net.outputpreactivation[k]-maxvalue);
        esum+=net.outputprobability[k];
    }
    for (int k=0;k<output_size;k++)
        net.outputprobability[k]/=esum;
}
int Bestindex(const Network& net)
{
    int bestindex=0;
    for (int k=1;k<output_size;k++)
    {
        if (net.outputprobability[k]>net.outputprobability[bestindex])
            bestindex=k;
    }
    return bestindex;
}
bool Check(char c)
{
    return c=='#'||c=='1'||c=='*';
}
bool Input(double (&input)[input_size])
{
    cout<<"输入8行8字符，#为笔画，quit退出\n";
    string line;
    for (int r=0;r<side;r++)
    {
        if (!safe_input(line))
            return false;
        if (r==0)
        {
            string cmd=line;
            if (!cmd.empty()&&cmd.back()=='\r')
                cmd.pop_back();
            if (cmd=="quit"||cmd=="exit"||cmd=="q")
                return false;
        }
        for (int c=0;c<side;c++)
        {
            char ch=c<(int)line.size()?line[c]:' ';
            input[r*side+c]=Check(ch)?1.0:0.0;
        }
    }
    return true;
}
void Centerimage(const double (&input)[input_size],double (&centered)[input_size])
{
    double sumR=0.0;
    double sumC=0.0;
    int ink=0;
    for (int r=0;r<side;r++)
    {
        for (int c=0;c<side;c++)
        {
            if (input[r*side+c]>0.5)
            {
                sumR+=r;
                sumC+=c;
                ++ink;
            }
        }
    }
    fill(centered,centered+input_size,0.0);
    if (!ink)
        return;
    double mid=(side-1)/2.0;
    int dr=(int)round(mid-sumR/ink);
    int dc=(int)round(mid-sumC/ink);
    for (int r=0;r<side;r++)
    {
        for (int c=0;c<side;c++)
        {
            if (input[r*side+c]<=0.5)
                continue;
            int nr=r+dr;
            int nc=c+dc;
            if (nr>=0&&nr<side&&nc>=0&&nc<side)
                centered[nr*side+nc]=1.0;
        }
    }
}
double Accuracy(Network& net,double images[][input_size],const unsigned char index[],int count)
{
    if (count<=0)
        return 0.0;
    int correct=0;
    for (int i=0;i<count;i++)
    {
        Forward(net,images[i]);
        if (Bestindex(net)==(int)index[i])
            ++correct;
    }
    return (double)correct/count;
}
int Loaddata(const string &directory,int maxcount,double image[][input_size],unsigned char index[])
{
    ifstream file(directory);
    if (!file)
    {
        cout<<"无法打开文件: "<<directory<<"\n";
        return -1;
    }
    string line;
    int count=0;
    while (count<maxcount&&getline(file,line))
    {
        if (!line.empty()&&line.back()=='\r')
            line.pop_back();
        if (line.empty())
            continue;
        index[count]=('0'<=line[0]&&line[0]<='9')?(unsigned char)(line[0]-'0'):0;
        for (int row=0;row<side;row++)
        {
            if (!getline(file,line))
            {
                cout<<"输入文件异常\n";
                return -1;
            }
            if (!line.empty()&&line.back()=='\r')
                line.pop_back();
            for (int col=0;col<side;col++)
            {
                char ch=col<(int)line.size()?line[col]:' ';
                image[count][row*side+col]=Check(ch)?1.0:0.0;
            }
        }
        double centered[input_size];
        Centerimage(image[count],centered);
        for (int i=0;i<input_size;i++)
            image[count][i]=centered[i];
        ++count;
    }
    return count;
}
void InitNet(Network& net,mt19937 &random_engine)
{
    double Inputrand=sqrt(6.0/input_size);
    double Hiddenrand=sqrt(6.0/hidden_size);
    uniform_real_distribution<double> dist_input(-Inputrand,Inputrand);
    uniform_real_distribution<double> dist_hidden(-Hiddenrand,Hiddenrand);
    for (int i=0;i<input_size;i++)
    {
        for (int j=0;j<hidden_size;j++)
            net.weighttohidden[i][j]=dist_input(random_engine);
    }
    for (int j=0;j<hidden_size;j++)
    {
        net.biashidden[j]=0.0;
        for (int k=0;k<output_size;k++)
            net.weighttooutput[j][k]=dist_hidden(random_engine);
    }
    for (int k=0;k<output_size;k++)
        net.biasoutput[k]=0.0;
}
double Trainone(Network& net,const double input[input_size],int correctindex,double learnrate)
{
    Forward(net,input);
    double outputdelta[output_size];
    for (int k=0;k<output_size;k++)
    {
        double target=(k==correctindex)?1.0:0.0;
        outputdelta[k]=net.outputprobability[k]-target;
    }
    double hiddendelta[hidden_size];
    for (int j=0;j<hidden_size;j++)
    {
        double backpropagation=0.0;
        for (int k=0;k<output_size;k++)
            backpropagation+=net.weighttooutput[j][k]*outputdelta[k];
        hiddendelta[j]=backpropagation*Activatek(net.hiddenpreactivation[j]);
    }
    double correctprobability=net.outputprobability[correctindex];
    if (correctprobability<1e-12)
        correctprobability=1e-12;
    double loss=-log(correctprobability);
    for (int j=0;j<hidden_size;j++)
    {
        for (int k=0;k<output_size;k++)
            net.weighttooutput[j][k]-=learnrate*net.hiddenvalue[j]*outputdelta[k];
    }
    for (int k=0;k<output_size;k++)
        net.biasoutput[k]-=learnrate*outputdelta[k];
    for (int i=0;i<input_size;i++)
    {
        for (int j=0;j<hidden_size;j++)
            net.weighttohidden[i][j]-=learnrate*input[i]*hiddendelta[j];
    }
    for (int j=0;j<hidden_size;j++)
        net.biashidden[j]-=learnrate*hiddendelta[j];
    return loss;
}
bool TrainNetwork(Network& net)
{
    cout<<"读取中...\n";
    int traincount=Loaddata(Datapath("train.txt"),train_capcity,train_image,train_index);
    if (traincount<=0)
        return false;
    int testcount=Loaddata(Datapath("test.txt"),test_capcity,test_image,test_index);
    if (testcount<=0)
        return false;
    mt19937 random_engine(sizeof("depress"));
    InitNet(net,random_engine);
    vector<int> order(traincount);
    for (int i=0;i<traincount;i++)
        order[i]=i;
    for (int epoch=1;epoch<=epochs;epoch++)
    {
        shuffle(order.begin(),order.end(),random_engine);
        double totalloss=0.0;
        int correct=0;
        for (int t=0;t<traincount;t++)
        {
            int idx=order[t];
            totalloss+=Trainone(net,train_image[idx],train_index[idx],learn_rate);
            if (Bestindex(net)==(int)train_index[idx])
                ++correct;
            if ((t+1)%20000==0||t+1==traincount)
            {
                cout<<"第"<<epoch<<"轮已完成"<<t+1<<"/"<<traincount<<"\n";
            }
        }
        double trainaccuracy=(double)correct/traincount;
        double testaccuracy=Accuracy(net,test_image,test_index,testcount);
        cout<<"第 "<<epoch<<" 轮结束：平均误差 "<<totalloss/traincount<<"，训练集正确率 "<<trainaccuracy*100.0<<"%，测试集正确率 "<<testaccuracy*100.0<<"%\n";
    }
    return true;
}
bool Loadweights(Network& net)
{
    ifstream file(Datapath("weights.txt"));
    if (!file)
        return false;
    for (int i=0;i<input_size;i++)
    {
        for (int j=0;j<hidden_size;j++)
        {
            if (!(file>>net.weighttohidden[i][j]))
                return false;
        }
    }
    for (int j=0;j<hidden_size;j++)
    {
        if (!(file>>net.biashidden[j]))
            return false;
    }
    for (int j=0;j<hidden_size;j++)
    {
        for (int k=0;k<output_size;k++)
        {
            if (!(file>>net.weighttooutput[j][k]))
                return false;
        }
    }
    for (int k=0;k<output_size;k++)
    {
        if (!(file>>net.biasoutput[k]))
            return false;
    }
    return true;
}
void Print(const double (&input)[input_size])
{
    for (int r=0;r<side;r++)
    {
        for (int c=0;c<side;c++)
            cout<<(input[r*side+c]>0.5?'#':'.');
        cout<<"\n";
    }
}
void Saveweights(Network& net)
{
    string path=Datapath("weights.txt");
    ofstream file(path);
    if (!file)
    {
        cout<<"无法保存权重到文件: "<<path<<"\n";
        return;
    }
    file.precision(17);
    for (int i=0;i<input_size;i++)
    {
        for (int j=0;j<hidden_size;j++)
            file<<net.weighttohidden[i][j]<<" ";
    }
    for (int j=0;j<hidden_size;j++)
        file<<net.biashidden[j]<<" ";
    for (int j=0;j<hidden_size;j++)
    {
        for (int k=0;k<output_size;k++)
            file<<net.weighttooutput[j][k]<<" ";
    }
    for (int k=0;k<output_size;k++)
        file<<net.biasoutput[k]<<" ";
    file<<"\n";
    if (!file)
        cout<<"保存权重时发生错误\n";
}
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    InitProgramDirectory();
    cout<<"神经网络手写数字识别目录:"<<programdirectory<<"\n";
    Network net;
    if (Loadweights(net))
    {
        cout<<"加载权重成功\n";
    }
    else
    {
        cout<<"加载权重失败，开始训练\n";
        if (!TrainNetwork(net))
        {
            cout<<"训练失败\n";
            return 1;
        }
        Saveweights(net);
    }
    double input[input_size];
    while (Input(input))
    {
        double centered[input_size];
        Centerimage(input,centered);
        bool blank=true;
        for (int i=0;i<input_size;i++)
        {
            if (centered[i]>0.5)
            {
                blank=false;
                break;
            }
        }
        if (blank)
        {
            cout<<"输入为空白图案，请重新输入\n";
            continue;
        }
        Forward(net,centered);
        int ans=Bestindex(net);
        cout<<"你画的是:(对齐后)\n";
        Print(centered);
        cout<<"识别结果: "<<ans<<" 把握度: "<<net.outputprobability[ans]*100.0<<"%\n";
        cout<<"各数字概率\n";
        for (int i=0;i<output_size;i++)
            cout<<i<<": "<<net.outputprobability[i]*100.0<<"%\n";
    }
    cout<<"退出程序\n";
    return 0;
}