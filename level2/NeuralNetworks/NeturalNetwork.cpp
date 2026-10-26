#include<bits/stdc++.h>
using namespace std;
const int side=8;
const int input_size=64;
const int hidden_size=64;
const int output_size=10;
const int learn_rate=0.1;
const int leaky_relu=0.01;
const int epochs=12;
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
double activate(double x)
{
    return (x>0)?x:leaky_relu*x;
}
double activatek(double x)
{
    return (x>0)?1:leaky_relu;
}
void forward(Network& net,const double input[input_size])
{
    for (int j=0;j<hidden_size;j++)
    {
        net.hiddenpreactivation[j]=net.biashidden[j];
        for (int i=0;i<input_size;i++)
            net.hiddenpreactivation[j]+=input[i]*net.weighttohidden[i][j];
        net.hiddenvalue[j]=activate(net.hiddenpreactivation[j]);
    }

    for (int k=0;k<output_size;k++)
    {
        net.outputpreactivation[k]=net.biasoutput[k];
        for (int j=0;j<hidden_size;j++)
            net.outputpreactivation[k]+=net.hiddenvalue[j]*net.weighttooutput[j][k];
    }

    double maxvalue=net.outputpreactivation[0];
    for (int k=1;k<output_size;k++)
        maxvalue=max(maxvalue,net.outputpreactivation[k]);

    double esum=0.0;
    for (int k=0;k<output_size;k++)
    {
        net.outputprobability[k]=exp(net.outputpreactivation[k]-maxvalue);
        esum+=net.outputprobability[k];
    }
    for (int k=0;k<output_size;k++)
    {
        net.outputprobability[k]/=esum;
    }
}

int bestindex(const Network& net)
{
    int bestindex=0;
    for (int k=1;k<output_size;k++)
        if (net.outputprobability[k]>net.outputprobability[bestindex])
            bestindex=k;
    return bestindex;
}

bool check(char c)
{
    return (c==' '||c=='.')?0:1;
}

bool input(double &input[input_size])
{
    cout << "输入8行8字符，#为笔画，quit退出\n";
    string line;
    for(int r=0;r<side;r++)
    {
        if(!getline(cin,line)) return 0;
        if(r==0)
        {
            string cmd=line;
            if(!cmd.empty()&&cmd.back()=='\r') cmd.pop_back();
            if(cmd=="quit"||cmd=="exit"||cmd=="q") return 0;
        }
        for(int c=0;c<side;c++)
        {
            char ch = c<(int)line.size() ? line[c] : ' ';
            input[r*side + c] = check(ch) ? 1.0 : 0.0;
        }
    }
    return 1;
}

void centerimage(const double &input[input_size],const double &centered[input_size])
{
    double sumR=0, sumC=0; int ink=0;
    for(int r=0;r<side;r++)
        for(int c=0;c<side;c++)
            if(input[r*side + c]>0.5)
                sumR+=r, sumC+=c, ++ink;

    fill(centered, centered+side, 0.0);
    if(!ink) return;

    double mid = (side-1)/2.0;
    int dr = round(mid - sumR/ink);
    int dc = round(mid - sumC/ink);

    for(int r=0;r<side;r++)
        for(int c=0;c<side;c++)
        {
            if(input[r*side + c]<=0.5) continue;
            int nr=r+dr, nc=c+dc;
            if(nr>=0&&nr<side&&nc>=0&&nc<side)
                centered[nr*side + nc]=1.0;
        }
}

string programdirectory;

double accuracy(Network& net)//?
{
   int correct=0, total=0;
}

void loaddata(Network& net)//?
{
    ifstream fin(programdirectory+"/train.txt");
    if(!fin) return;
    for(int e=0;e<epochs;e++)
    {
        string line;
        while(getline(fin,line))
        {
            if(line.empty()) continue;
            int label=line[0]-'0';
            double input[input_size];
            for(int r=0;r<side;r++)
            {
                if(!getline(fin,line)) return;
                for(int c=0;c<side;c++)
                    input[r*side + c] = check(c<(int)line.size() ? line[c] : ' ') ? 1.0 : 0.0;
            }
            forward(net,input);
        }
        fin.clear();
        fin.seekg(0,ios::beg);
    }
}
void init(Network& net)//?
{
    srand(time(0));
    for (int i=0;i<input_size;i++)
        for (int j=0;j<hidden_size;j++)
            net.weighttohidden[i][j]=((double)rand())/RAND_MAX*2.0-1.0;
    for (int j=0;j<hidden_size;j++)
        net.biashidden[j]=((double)rand())/RAND_MAX*2.0-1.0;
    for (int j=0;j<hidden_size;j++)
        for (int k=0;k<output_size;k++)
            net.weighttooutput[j][k]=((double)rand())/RAND_MAX*2.0-1.0;
    for (int k=0;k<output_size;k++)
        net.biasoutput[k]=((double)rand())/RAND_MAX*2.0-1.0;
}
bool train(Network& net)//?
{
    ifstream fin(programdirectory+"/train.txt");
    if(!fin) return 0;
    for(int e=0;e<epochs;e++)
    {
        string line;
        while(getline(fin,line))
        {
            if(line.empty()) continue;
            int label=line[0]-'0';
            double input[input_size];
            for(int r=0;r<side;r++)
            {
                if(!getline(fin,line)) return 0;
                for(int c=0;c<side;c++)
                    input[r*side + c] = check(c<(int)line.size() ? line[c] : ' ') ? 1.0 : 0.0;
            }
            forward(net,input);
            double target[output_size]={};
            target[label]=1.0;

            double outputdelta[output_size];
            for(int k=0;k<output_size;k++)
                outputdelta[k]=net.outputprobability[k]-target[k];

            double hiddendelta[hidden_size]={};
            for(int j=0;j<hidden_size;j++)
                for(int k=0;k<output_size;k++)
                    hiddendelta[j]+=outputdelta[k]*net.weighttooutput[j][k]*activatek(net.hiddenpreactivation[j]);

            for(int j=0;j<hidden_size;j++)
                for(int k=0;k<output_size;k++)
                    net.weighttooutput[j][k]-=learn_rate*outputdelta[k]*net.hiddenvalue[j];
            for(int k=0;k<output_size;k++)
                net.biasoutput[k]-=learn_rate*outputdelta[k];

            for(int i=0;i<input_size;i++)
                for(int j=0;j<hidden_size;j++)
                    net.weighttohidden[i][j]-=learn_rate*hiddendelta[j]*input[i];
            for(int j=0;j<hidden_size;j++)
                net.biashidden[j]-=learn_rate*hiddendelta[j];
        }
        fin.clear();
        fin.seekg(0,ios::beg);
    }
    return 1;
}

void loadweights(Network& net)//?
{
    ifstream fin(programdirectory+"/weights.txt");
    for (int i=0;i<input_size;i++)
        for (int j=0;j<hidden_size;j++)
            fin>>net.weighttohidden[i][j];
    for (int j=0;j<hidden_size;j++)
        fin>>net.biashidden[j];
    for (int j=0;j<hidden_size;j++)
        for (int k=0;k<output_size;k++)
            fin>>net.weighttooutput[j][k];
    for (int k=0;k<output_size;k++)
        fin>>net.biasoutput[k];
}
void print(const double &input[input_size])
{
    for(int r=0;r<side;r++)
    {
        for(int c=0;c<side;c++)
        {
            cout << (input[r*side + c] > 0.5 ? '#' : '.');
        }
        cout << "\n";
    }
}

int main()
{
    cout<<"各数字概率\n";
    for (int i=0;i<output_size;i++)
        cout<<i<<": "<<net.outputprobability[i]<<"\n";
    return 0;
}