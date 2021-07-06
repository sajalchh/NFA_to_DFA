// Sajal Chhamunya
// 19CS10051

#include <iostream>
#include <fstream>

using namespace std;

unsigned int binpow(unsigned int a, unsigned int b) {
    unsigned int res = 1;
    while (b > 0) {
        if (b & 1)
            res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

class NFA{
    public:
    int n,m;
    unsigned int start,final;
    unsigned int **delta;
    NFA():n(0),start(0),final(0){}
    void readNFA(string file){
        ifstream in;
        in.open(file);
        if(!in){
            cerr<<"File couldn't be opened. Try again"<<endl;
            exit(1);
        }
        in>>n>>m;
        int temp=0,a,b;
        do
        {
            in>>temp;
            if(temp!=-1){
                start|=(1U << temp);
            }
        } while (temp!=-1);
        temp=0;
        do
        {
            in>>temp;
            if(temp!=-1){
                final|=(1U << temp);
            }
        } while (temp!=-1);
        delta=new unsigned int*[n];
        for (int i = 0; i < n; i++)
        {
            delta[i]=new unsigned int[m];
            for(int j=0;j<m;j++){
                delta[i][j]=0;
            }
        }
        temp=0;
        a=0;
        do
        {
            in>>a>>b>>temp;
            if(a!=-1){
                delta[a][b]|=(1U << temp);
            }
        } while (a!=-1);
    }
    void printNFA(){
        cout<<"    Number of states: "<<n<<endl;
        cout<<"    Input alphabet: {0";
        for(int i=1;i<this->m;i++){
            cout<<","<<i;
        }
        cout<<"}"<<endl<<"    Start states: {";
        unsigned int temp=start,a=1U;
        for(int i=0;i<32;i++,a=a<<1){
            if(temp&a){
                cout<<i;
                if(temp-a<=a){
                    cout<<"}"<<endl;
                    break;
                }
                else{
                    cout<<",";
                }
            }
        }
        cout<<"    Final states: {";
        temp=final;
        a=1U;
        for(int i=0;i<32;i++,a=a<<1){
            if(temp&a){
                cout<<i;
                if(temp-a<=a){
                    cout<<"}"<<endl;
                    break;
                }
                else{
                    cout<<",";
                }
            }
        }
        cout<<"    Transition function"<<endl;
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                temp=(this->delta)[i][j];
                a=1U;
                cout<<"        Delta("<<i<<","<<j<<") = {";
                if(temp==0)
                cout<<"}"<<endl;
                else{
                    for(int k=0;k<32;k++,a=a<<1){
                        if(temp&a){
                            cout<<k;
                            if(temp-a<=a){
                                cout<<"}"<<endl;
                                break;
                            }
                            else{
                                cout<<",";
                            }
                        }
                    }
                }
            }
        }




    }

};

class states{
    public:
    int size;
    unsigned int*a;
    states():size(0),a(NULL){}
    states(int n):size(n){
        a=new unsigned int[size];
    }
    states updateState(int n){
        size=n/32+1;
        a=new unsigned int[size];
        for(int i=0;i<size;i++){
            a[i]=0;
        }
        return *this;
    }
    states make1(int n){
        for(int i=0;i<size-1;i++){
            a[i]=~(a[i]&0);
        }
        // cout<<a[0];
        n=n%32;
        unsigned int b=1U;
        for(int i=0;i<n;i++,b=b<<1){
            a[size-1]|=b;
        }
        return *this;
    }
    bool checkState (int n){
        int k=n/32;
        n%=32;
        return (a[k]&(1U<<n)); 
    }
    void setState(int n){
        int k=n/32;
        n%=32;
        a[k]|=(1U<<n);
    }

    states operator |(states const &s){
        states ans(s.size);
        for(int i=0;i<size;i++){
            ans.a[i]=a[i]|s.a[i];
        }
        return ans;
    }

    int count(){
        int ans=0;
        unsigned int b=1U;
        for(int i=0;i<size;i++){
            b=1U;
            for(int j=0;j<32;j++,b=b<<1){
                if(a[i]&b){
                    ans++;
                }
            }
        }
        return ans;
    }
};

class DFA{
    public:
    unsigned int n,m,s;
    unsigned int **delta;
    states q,f;
    DFA(){}
    DFA subsetcons(NFA N){
        n=binpow(2,N.n);
        m=N.m;
        s=N.start;
        q.updateState(n);
        q.make1(n);
        delta=new unsigned int*[n];
        unsigned int temp,a;
        for(int i=0;i<n;i++){
            delta[i]=new unsigned int[m];
            for(int j=0;j<m;j++){
                delta[i][j]=0;
            }
        }
        f.updateState(n);
        for(int i=0;i<n;i++){
            if(i&N.final){
                f.setState(i);
            }
        }
        for(int i=0;i<n;i++){
            unsigned int temp=i,a=1U;
            for(int j=0;j<32;j++,a=a<<1){
                if(temp&a){
                    for(int k=0;k<m;k++){
                        delta[i][k]|=N.delta[j][k];
                        // cout<<N.delta[j][k]<<" ";
                    }
                    if(temp-a<=a){
                        break;
                    }
                }
            }
        }
        return *this;
    }
    void printDFA(){
        cout<<"    Number of states: "<<this->n<<endl;
        cout<<"    Input alphabet: {0";
        for(int i=1;i<this->m;i++){
            cout<<","<<i;
        }
        cout<<"}"<<endl<<"    Start state: {"<<this->s<<"}"<<endl;
        unsigned int a=1U;
        states temp;

        int c=f.count();
        if(c<64){
            cout<<"    Final states: {";
            a=1U;
            for(int i=0;i<n;i++){
                if(f.checkState(i)){
                    cout<<i<<",";
                }
            }
            cout<<"}"<<endl;
        }
        else{
            cout<<"    "<<c<<" Final states"<<endl;
        }
        cout<<"    Transition function";
        if(n<64){
            cout<<endl<<"    a\\p| ";
            for(int i=0;i<n;i++){
                if(i<10)
                cout<<" ";
                cout<<" "<<i<<" ";
                
            }
            cout<<endl<<"    ---+-";
            for(int i=0;i<n;i++){
                cout<<"----";
            }
            cout<<endl;
            for(int i=0;i<m;i++){
                cout<<"     "<<i<<" | ";
                for(int j=0;j<n;j++){
                    if(delta[j][i]<10)
                    cout<<" ";
                    cout<<" "<<delta[j][i]<<" ";
                    
                }
                cout<<endl;
            }
        }
        else{
            cout<<": Skipped"<<endl;
        }
    }
};
void findreachable(DFA &D,states &r,unsigned int a){
    if(r.checkState(a)!=0){
        return;
    }
    else{
        r.setState(a);
        for(int i=0;i<D.m;i++){
            findreachable(D,r,D.delta[a][i]);
        // cout<<D.delta[a][1]<<" "<<a<<" ";
        }
    }
}
DFA rmunreachable(DFA &D,states &r){
    int n=r.count();
    DFA nD;
    nD.n=n;
    nD.m=D.m;
    nD.delta=new unsigned int*[n];
    for(int i=0;i<n;i++){
        nD.delta[i]=new unsigned int[nD.m];
        for(int j=0;j<nD.m;j++){
            nD.delta[i][j]=0;
        }
    }
    nD.q.updateState(n);
    nD.q.make1(n);

    int j=0;
    unsigned int x[n];
    for(int i=0;i<nD.n;i++){
        while(!r.checkState(j)){
            j++;
        }
        x[i]=j;
        j++;
    }
    for(int i=0;i<n;i++){
        if(x[i]==D.s){
            nD.s=i;
            break;
        }
    }
    unsigned int tmp;
    for(int i=0;i<n;i++){
        for(int k=0;k<nD.m;k++){
            tmp=D.delta[x[i]][k];
            j=0;
            while(x[j]!=tmp){
                j++;
            }
            nD.delta[i][k]=j;
        }
    }
    nD.f.updateState(n);
    for (int i = 0; i < n; i++)
    {
        if(D.f.checkState(x[i])!=0){
            nD.f.setState(i);
        }
    }
    // cout<<nD.f.count()<<endl;
    return nD;
    
}

int** findequiv(DFA D){
    int **M=new int*[D.n];
    for(int i=0;i<D.n;i++){
        M[i]=new int[D.n];
    }
    int k=D.f.count();
    unsigned int final[k];
    unsigned int count=0;
    unsigned int c=0;
    for(int i=0;i<D.n;i++){
            if(D.f.checkState(i)){
                final[c]=i;
                c++;
            }
    }
    for(int i=0;i<D.n;i++){
        for(int j=0;j<k;j++){
            M[i][final[j]]=1;
            M[final[j]][i]=1;
        }
    }
    for(int i=0;i<k;i++){
        for(int j=i;j<k;j++){
            M[final[i]][final[j]]=0;
            M[final[j]][final[i]]=0;
        }
    }
    // for(int i=0;i<D.n;i++){
    //     for(int j=0;j<=i;j++){
    //         cout<<M[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    //Initialisation done
    bool chk;
    do{
        chk=false;
        for(int i=0;i<D.n;i++){
            for(int j=0;j<=i;j++){
                if(M[i][j]==0){
                    for(int k=0;k<D.m;k++){
                        if(M[D.delta[i][k]][D.delta[j][k]]==1){
                            M[i][j]=1;
                            M[j][i]=1;
                            chk=true;
                        }
                    }
                }
            }
        }
    }
    while(chk);
    
    return M;
    

}

DFA compress(DFA &D,int** &M){
    int a[D.n],b[D.n];
    for(int i=0;i<D.n;i++){
        a[i]=i;
        b[i]=0;
    }
    for(int i=0;i<D.n;i++){
        for(int j=0;j<=i;j++){
            if(M[i][j]==0){
                for(int k=0;k<D.n;k++){
                    if(a[k]==i){
                        a[k]=j;
                    }
                }
            }
        }
    }
    int count = 0;
    cout<<endl<<"+++ Equivalent states"<<endl;
    for(int i=0;i<D.n;i++){
        if(b[a[i]]==0){
            b[a[i]]=1;
            cout<<"    Group "<<count<<": {"<<i;
            for(int j=0;j<D.n;j++){
                if(a[j]==a[i]&&i!=j){
                    cout<<","<<j;
                }
            }
            cout<<"}"<<endl;
            count++;
        }
    }
    int x[count];
    int c=0;
    for(int j=0;j<D.n;j++){
        if(a[j]==j){
            x[c]=j;
            c++;
        }
    }
    DFA nD;
    int n=count;
    nD.n=n;
    nD.m=D.m;
    for(int i=0;i<n;i++){
        if(x[i]==a[D.s]){
            
            nD.s=i;
            break;
        }
    }
    nD.f.updateState(n);
    for (int i = 0; i < D.n; i++)
    {
        if(D.f.checkState(i)!=0){
            for(int j=0;j<count;j++){
                if(a[i]==x[j]){
                    nD.f.setState(j);
                }
            }
        }
    }
    
    nD.delta=new unsigned int*[n];
    for(int i=0;i<n;i++){
        nD.delta[i]=new unsigned int[nD.m];
        for(int j=0;j<nD.m;j++){
            nD.delta[i][j]=0;
        }
    }
    nD.q.updateState(n);
    nD.q.make1(n);
    int j=0;
    unsigned int tmp;
    for(int i=0;i<n;i++){
        for(int k=0;k<nD.m;k++){
            while(x[i]!=a[j]){
                j++;
            }
            tmp=D.delta[j][k];
            for(int d=0;d<n;d++){
                if(a[tmp]==x[d]){
                    nD.delta[i][k]=d;
                }
            }
        }
    }
    return nD;
    
}

int main(){
    string file;
    cout<<"Enter the name of the input file:"<<endl;
    cin>>file;
    NFA N;
    N.readNFA(file);
    cout<<"+++ Input NFA"<<endl;
    N.printNFA();
    DFA D;
    D.subsetcons(N);
    cout<<endl<<"+++ Converted DFA"<<endl;
    D.printDFA();
    states r;
    r.updateState(D.n);
    findreachable(D,r,D.s);
    DFA nD=rmunreachable(D,r);
    cout<<endl<<"+++ Reachable states: {";
    unsigned int j=0;
    for(int i=0;i<nD.n;i++){
        while(!r.checkState(j)){
            j++;
        }
        cout<<j<<",";
        j++;
    }
    cout<<"}"<<endl;
    cout<<endl<<"+++ Reduced DFA after removing unreachable states"<<endl;
    nD.printDFA();
    int **M=findequiv(nD);
    DFA mnD=compress(nD,M);
    cout<<endl<<"+++ Reduced DFA after collapsing equivalent states"<<endl;
    mnD.printDFA();
    return 0;
}