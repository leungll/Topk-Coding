#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int MAXN = 1e4 + 10;

#define LOCAL

//Graph
bool g[MAXN][MAXN], update_g[MAXN][MAXN], new_g[MAXN][MAXN];
//A vertex can only appear once in a independent set
bool vertex_flag[MAXN];
//Vertex's degree
map<int, int> degree_mp;
//Vertex produce hard clause
vector<int> hard_clause_vertex;
//Vertex produce soft clause, Independent set of vertex
vector<ll> soft_clause_vertex, independentSet[MAXN];
//Delete vertex and it's neighbour by degree
vector<int> deleteVertexByDegree_vector_temp;
set<int> deleteVertexByDegree_set;
bool degree_vertex_vis[MAXN];
//Keep the vertices in G
vector<int> retain_n;
//Hard clause martix
bool hard_clause_martix_graph_temp[MAXN][MAXN];
vector<int> hard_clause_martix_graph_vector[MAXN];
//Soft clause
set<int> soft_clause_set[MAXN];
set<int>::iterator it;
bool soft_clause_setVis[MAXN], soft_clause_vertex_vis[MAXN];
//Unit Clause
vector<int> unit_clause_vector;
//G' vertex
bool deleteVertexAndNeighbor[MAXN];
//New_g Vertex
set<int> new_g_vertex_set;
bool new_g_vertex_vis[MAXN];
//Reduce rules
bool reduce_vertex_vis[MAXN];
vector<int> reduceVertexByDegree_0_vector_temp;
vector<int> reduceVertexByDegree_1_vector_temp;
vector<int> reduceVertexByDegree_2_vector_temp;
vector<int> reduceEdges[MAXN];
//AllNum of KplexNumResults
vector<int> kplexNumResults_vector[MAXN];
map<int, int> mp_kplex_size;

//Number of Vertex
int ver;
//Number of Edge 
ll edge;
//Number of Hard Clause, Number of Soft Clause
ll hard_count, soft_count;
ll reduceEdges_cnt = 0;
ll allNum = 0;
int verFlag = 0, vNum = 0;

vector<string> Split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	char * strs = new char[str.length() + 1];
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p;
		res.push_back(s);
		p = strtok(NULL, d);
	}
	return res;
}

void ReadFile(string fileName){
    string str;
    //fstream in("./V5.wclq", ios::in);
    fstream in(fileName);
    if(!in) {
        cout << "Read file error from ReadFile!" << endl;
    }
    while(getline(in, str)) {
        if(str[0] == 'c'){
            continue;
        }else if(str[0] == 'p'){
            vector<string> res = Split(str, " ");
            ver = atoi(res[2].c_str());
            edge = atoi(res[3].c_str());

            cout << "ver : " << ver << " edge : " << edge << endl;
        }else if(str[0] == 'e'){
            vector<string> res = Split(str, " ");
            // int len = res.size();

            int x, y;
            x = atoi(res[1].c_str());
            y = atoi(res[2].c_str());

            // string t1 = res[len - 1];
            // stringstream ss1;
            // ss1 << t1;
            // ss1 >> x;

            // string t2 = res[len - 2];
            // stringstream ss2;
            // ss2 << t2;
            // ss2 >> y;

            new_g_vertex_set.insert(x);
            new_g_vertex_set.insert(y);
            g[x][y] = g[y][x] = true;
        }
    }
}

int IndependentSet(){
    int independent_count = 0;
    independentSet[0].clear();
    independentSet[independent_count++].push_back(soft_clause_vertex[0]);
    for(int i = 1; i < soft_clause_vertex.size(); i++) {
        int now = soft_clause_vertex[i];
        int cnt = 0;
        for(int j = 0; j < independent_count; j++) {
            int flag = 1;
            for(int k = 0; k < independentSet[j].size(); k++) {
                int nw = independentSet[j][k];
                if(g[nw][now]) {
                    flag = 0;
                    break;
                }
            }
            if(flag) {
                if(!vertex_flag[now]){
                    independentSet[j].push_back(now);
                    vertex_flag[now] = true;
                }
            } else {
                cnt++;
            }
        }
        if(cnt == independent_count) {
            if(!vertex_flag[now]){
                independentSet[independent_count++].push_back(now);
                vertex_flag[now] = true;
            }
        }
    }
    return independent_count;
}

int Check(int independentCount){
    int cnt = 0;
    for(int i = 0;i < independentCount;i++){
        if(!soft_clause_setVis[i]){
            if(soft_clause_set[i].size() == 1){
                unit_clause_vector.push_back(i);
                cnt++;
                soft_clause_setVis[i] = true;
            }
        }
    }
    if(unit_clause_vector.size() == 0){
        for(int i = 0;i < independentCount;i++){
            if(!soft_clause_setVis[i]){
                unit_clause_vector.push_back(i);
                cnt++;
                soft_clause_setVis[i] = true;
                break;
            }
        }
    }
    return cnt;
}

int IncongruentClauseSetUpperBound(int independentCount){
    int cnt = 0, size;
    while( size = Check(independentCount) ){
        // printf("size: %d\n", size);

        //count
        for(int i = 0; i < size; i++) {
            int idx = unit_clause_vector[i];
            it = soft_clause_set[idx].begin();
            int now = *it;

            // printf("countnow: %d\n", now);

            for(int j = i + 1;j < size;j++){
                int idx = unit_clause_vector[j];
                it = soft_clause_set[idx].begin();
                int next = *it;

                // printf("countnext: %d\n", next);

                for(int k = 0;k < hard_clause_martix_graph_vector[now].size();k++){
                    if(next == hard_clause_martix_graph_vector[now][k]){
                        cnt++;
                    }
                }
            }
        }
        
        //delete
        for(int i = 0;i < size;i++){
            int idx = unit_clause_vector[i];
            it = soft_clause_set[idx].begin();
            int now = *it;

            // printf("delete vertex now: %d\n", now);

            if(!soft_clause_vertex_vis[now]){
                for(int b = 0;b < independentCount;b++){
                    if(soft_clause_set[b].count(now)){
                        soft_clause_set[b].erase(now);
                    }
                }
            }
            soft_clause_vertex_vis[now] = true;

            // puts("----");
            for(int a = 0;a < hard_clause_martix_graph_vector[now].size();a++){
                int temp = hard_clause_martix_graph_vector[now][a];

                // printf("delete vertex: %d\n", temp);

                if(!soft_clause_vertex_vis[temp]){
                    for(int b = 0;b < independentCount;b++){
                        if(soft_clause_set[b].count(temp)){
                            soft_clause_set[b].erase(temp);
                        }
                    }
                }
                soft_clause_vertex_vis[temp] = true;
            }
        }
        unit_clause_vector.clear();
        // puts("");
    }
    #ifdef LOCAL
    cout << "IncongruentClauseSetUpperBound's cnt : " << cnt << endl;
    #endif
    return (independentCount - cnt);
}

int DeleteVertexByDegree(int r){
    int cnt = 0;
    for(int i = 1;i <= ver;i++){
        if(!reduce_vertex_vis[i]){
            if(new_g_vertex_vis[i]){
                if( (degree_mp[i] < r - 1) && (!degree_vertex_vis[i])){
                    deleteVertexByDegree_vector_temp.push_back(i);
                    deleteVertexByDegree_set.insert(i);
                    for(int j = 1;j <= ver;j++){
                        if(g[i][j] && g[j][i]){
                            deleteVertexByDegree_set.insert(j);
                        }
                    }
                    cnt++;
                    degree_vertex_vis[i] = true;
                }
            }
        }
    }
    return cnt;
}

void DegreeUpdateG(){
    for(int i = 1;i <= ver;i++){
        if(!reduce_vertex_vis[i]){
            if(new_g_vertex_vis[i]){
                for(int j = 1;j <= ver;j++){
                    if(new_g_vertex_vis[j]){
                        if(i == j){
                            continue;
                        }
                        if(update_g[i][j] && update_g[j][i]){
                            degree_mp[i]++;
                        }
                    }
                }
            }
        }
    }
}

void Prune(int bound){
    #ifdef LOCAL
    puts("-------------------------");
    cout << "start prune : " << endl;
    #endif

    int size;
    while(size = DeleteVertexByDegree(bound)){
        #ifdef LOCAL
        cout << "size : " << size << endl;
        #endif

        for(int i = 0;i < size;i++){
            int temp = deleteVertexByDegree_vector_temp[i];

            #ifdef LOCAL
            cout << "deleteVertexByDegree_temp is : " << temp << endl;
            #endif

            for(int j = 1;j <= ver;j++){
                if(!reduce_vertex_vis[i]){
                    if(new_g_vertex_vis[j]){
                        update_g[temp][j] = update_g[j][temp] = false;
                    }
                }
            }
            degree_mp.clear();
            DegreeUpdateG();
        }
        deleteVertexByDegree_vector_temp.clear();
        #ifdef LOCAL
        puts("");
        #endif
    }
}

int Degree_0(){
    int cnt = 0;
    for(int i = 1;i <= ver;i++){
        if(new_g_vertex_vis[i]){
            if( (degree_mp[i] == 0 && !reduce_vertex_vis[i]) ){
                reduceVertexByDegree_0_vector_temp.push_back(i);
                cnt++;
                reduce_vertex_vis[i] = true;
            }
        }
    }
    return cnt;
}

int Degree_1(){
    int cnt = 0;
    for(int i = 1;i <= ver;i++){
        if(new_g_vertex_vis[i]){
            if( (degree_mp[i] == 1 && !reduce_vertex_vis[i]) ){
                reduceVertexByDegree_1_vector_temp.push_back(i);
                cnt++;
                reduce_vertex_vis[i] = true;
            }
        }
    }
    return cnt;
}

int Degree_2(){
    int cnt = 0;
    for(int i = 1;i <= ver;i++){
        if(new_g_vertex_vis[i]){
            if( (degree_mp[i] == 2 && !reduce_vertex_vis[i]) ){
                vector<int> reduce_vertex_neighbor;
                reduce_vertex_neighbor.clear();
                for(int j = 1;j <= ver;j++){
                    if(new_g_vertex_vis[j]){
                        if(g[i][j] && g[j][i]){
                            reduce_vertex_neighbor.push_back(j);
                        }
                    }
                }
                int ni = reduce_vertex_neighbor[0];
                int nj = reduce_vertex_neighbor[1];
                if(!g[ni][nj] && !g[nj][ni]){
                    reduceVertexByDegree_2_vector_temp.push_back(i);
                    cnt++;
                    reduce_vertex_vis[i] = true;
                }
            }
        }
    }
    return cnt;
}

void DegreeG(){
    for(int i = 1;i <= ver;i++){
        if(new_g_vertex_vis[i]){
            for(int j = 1;j <= ver;j++){
                if(new_g_vertex_vis[j]){
                    if(i == j){
                        continue;
                    }
                    if(g[i][j] && g[j][i]){
                        degree_mp[i]++;
                    }
                }
            }
        }
    }
}

void Reduce(){
    #ifdef LOCAL
    puts("-------------------------");
    cout << "start reduce : " << endl;
    #endif

    int size0, size1, size2;
    int flag1, flag2;
    do{
        flag1 = 0, flag2 = 0;
        // puts("");
        for(int i = 1;i <= ver;i++){
            if(new_g_vertex_vis[i]){
                // cout << i << " " << reduce_vertex_vis[i] << endl;
                if(reduce_vertex_vis[i] == 0){
                    flag1++;
                }
            }
        }
        while(size0 = Degree_0()){
            #ifdef LOCAL
            cout << "size0 : " << size0 << endl;
            #endif

            for(int i = 0;i < size0;i++){
                int temp = reduceVertexByDegree_0_vector_temp[i];

                #ifdef LOCAL
                cout << "reduceVertexByDegree_0_temp is : " << temp << endl;
                #endif

                degree_mp.clear();
                DegreeG();
            }
            reduceVertexByDegree_0_vector_temp.clear();

            #ifdef LOCAL
            puts("");
            #endif
        }
        while(size1 = Degree_1()){
            #ifdef LOCAL
            cout << "size1 : " << size1 << endl;
            #endif

            for(int i = 0;i < size1;i++){
                int temp = reduceVertexByDegree_1_vector_temp[i];

                #ifdef LOCAL
                cout << "reduceVertexByDegree_1_temp is : " << temp << endl;
                #endif

                for(int j = 1;j <= ver;j++){
                    if(new_g_vertex_vis[j]){
                        if(g[temp][j] && g[j][temp]){
                            g[temp][j] = g[j][temp] = false;
                            reduceEdges_cnt++;
                            reduceEdges[reduceEdges_cnt].push_back(temp);
                            reduceEdges[reduceEdges_cnt].push_back(j);
                        }
                    }
                }
                degree_mp.clear();
                DegreeG();
            }
            reduceVertexByDegree_1_vector_temp.clear();

            #ifdef LOCAL
            puts("");
            #endif
        }
        while(size2 = Degree_2()){
            #ifdef LOCAL
            cout << "size2 : " << size2 << endl;
            #endif

            for(int i = 0;i < size2;i++){
                int temp = reduceVertexByDegree_2_vector_temp[i];

                #ifdef LOCAL
                cout << "reduceVertexByDegree_2_temp is : " << temp << endl;
                #endif 

                for(int j = 1;j <= ver;j++){
                    if(new_g_vertex_vis[j]){
                        if(g[temp][j] && g[j][temp]){
                            g[temp][j] = g[j][temp] = false;
                            reduceEdges_cnt++;
                            reduceEdges[reduceEdges_cnt].push_back(temp);
                            reduceEdges[reduceEdges_cnt].push_back(j);
                        }
                    }
                }
                degree_mp.clear();
                DegreeG();
            }
            reduceVertexByDegree_2_vector_temp.clear();
            #ifdef LOCAL
            puts("");
            #endif
        }
        // puts("");
        for(int i = 1;i <= ver;i++){
            if(new_g_vertex_vis[i]){
                // cout << i << " " << reduce_vertex_vis[i] << endl;
                if(reduce_vertex_vis[i] == 0){
                    flag2++;
                }
            }
        }
    }while(flag1 != flag2);
}

void StringReplace( std::string &strBig, const std::string &strsrc, const std::string &strdst) {
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while( (pos = strBig.find(strsrc, pos)) != std::string::npos ) {
		strBig.replace( pos, srclen, strdst );
		pos += dstlen;
	}
}

string GetPathOrURLShortName(std::string strFullName) {
	if (strFullName.empty()) {
		return "";
	}
	StringReplace(strFullName, "/", "\\");
	std::string::size_type iPos = strFullName.find_last_of('\\') + 1;
	return strFullName.substr(iPos, strFullName.length() - iPos);
}

int GetKplexNum(string s) {
	fstream fin(s.c_str(), ios::in);
	if(!fin) {
		cerr << "Can not open file from GetKplexNum!" <<endl;
		return -1;
	}
	char c;
	int lineCnt = 0;
	while(fin.get(c)) {
		if(c =='\n'){
			lineCnt++;
		}
	}
	fin.close();
	return lineCnt;
}

void GetKplexNumResults(string s, int kplexNum){
    fstream fin(s.c_str(), ios::in);
    if(!fin) {
		cerr << "Can not open file from GetKplexNumResults!" << endl;
	}
    for(int i = 0;i < kplexNum;i++){
        string temp;
        getline(fin, temp);
        allNum++;

        string str_input = temp;
        vector<int> nums;
        nums.clear();
        char *s_input = (char *)str_input.c_str();
        const char * split = "', '";
        char *p = strtok(s_input, split);
        int a;
        while(p != NULL) {
            sscanf(p, "%d", &a);
            nums.push_back(a);
            p = strtok(NULL, split);
        }
        for(int a = 1; a < nums.size() - 1; a++) {
            kplexNumResults_vector[allNum].push_back(nums[a]);
        }
    }
    fin.close();
}

bool cmp(const pair<int, int>& a, const pair<int, int>& b) {
    return a.second > b.second;
}

int main(int argc, char **argv) {

    int K = atoi(argv[2]);
    int _k = K;
    string strFilePath = argv[1];
    int fileCnt = 0;
    bool isReduce = false;

    memset(reduce_vertex_vis, false, sizeof(reduce_vertex_vis));

    while(true){
        ll S = 0;
        hard_count = 0, soft_count = 0;
        int g_edge = 0, update_g_edge = 0, newEdge = 0;

        //initialize
        memset(g, false, sizeof(g));
        memset(update_g, false, sizeof(update_g));
        memset(new_g, false, sizeof(new_g));
        memset(vertex_flag, false, sizeof(vertex_flag));
        memset(hard_clause_martix_graph_temp, false, sizeof(hard_clause_martix_graph_temp));
        memset(soft_clause_vertex_vis, false, sizeof(soft_clause_vertex_vis));
        memset(soft_clause_setVis, false, sizeof(soft_clause_setVis));
        memset(degree_vertex_vis, false, sizeof(degree_vertex_vis));
        memset(deleteVertexAndNeighbor, false, sizeof(deleteVertexAndNeighbor));
        memset(new_g_vertex_vis, false, sizeof(new_g_vertex_vis));
        
        retain_n.clear();
        degree_mp.clear();
        hard_clause_vertex.clear();
        soft_clause_vertex.clear();
        deleteVertexByDegree_vector_temp.clear();
        reduceVertexByDegree_0_vector_temp.clear();
        reduceVertexByDegree_1_vector_temp.clear();
        reduceVertexByDegree_2_vector_temp.clear();
        deleteVertexByDegree_set.clear();
        unit_clause_vector.clear();
        new_g_vertex_set.clear();

        for(int i = 1;i <= MAXN;i++){
            independentSet[i].clear();
            hard_clause_martix_graph_vector[i].clear();
            soft_clause_set[i].clear();
        }

        //read file
        #ifdef LOCAL
        puts("----------------------------------------------------------------------------");
        cout << "fileNamePath : " << strFilePath << endl;
        #endif
        ReadFile(strFilePath);
        string fileNameExtension = GetPathOrURLShortName(strFilePath);
        string fileName = fileNameExtension.substr(0, fileNameExtension.rfind("."));
        #ifdef LOCAL
        cout << "fileNameExtension : " << fileNameExtension << endl;
        cout << "fileName : " << fileName << endl;
        #endif

        #ifdef LOCAL
        puts("-------------------------");
        cout << "new_g_vertex_set : ";
        #endif
        for(it = new_g_vertex_set.begin(); it != new_g_vertex_set.end(); ++it) {
            #ifdef LOCAL
            printf("%d ", *it);
            #endif 
            new_g_vertex_vis[*it] = true;
        }
        #ifdef LOCAL
        puts("");
        #endif
        int new_g_vertex_num = 0;
        // cout << "new_g_vertex_vis : " << endl;
        for(int i = 1;i <= ver;i++){
            if(new_g_vertex_vis[i]){
                new_g_vertex_num++;
            }
            // cout << i << " " << new_g_vertex_vis[i] << endl;
        }
        #ifdef LOCAL
        cout << "new_g_vertex_num : " << new_g_vertex_num << endl;
        #endif

        //initialize degree_mp
        for(int i = 1;i <= ver;i++){
            degree_mp[i] = 0;
        }

        //original G
        // puts("");
        // puts("-------------------------");
        // cout << "G : " << endl;
        // for(int i = 1;i <= ver;i++){
        //     for(int j = 1;j <= ver;j++){
        //         cout << g[i][j] << " ";
        //     }
        //     puts("");
        // }

        //vertex's degree
        DegreeG();
        #ifdef LOCAL
        puts("-------------------------");
        cout << "start vertex's degree : " << endl;
        #endif
        int startVertexNum = 0;
        for(int i = 1;i <= ver;i++){
            if(!reduce_vertex_vis[i]){
                if(new_g_vertex_vis[i]){
                    #ifdef LOCAL
                        cout << i << " " << degree_mp[i] << " " << endl;
                    #endif
                    startVertexNum++;
                }
            }
        }
        #ifdef LOCAL
        cout << "startVertexNum : " << startVertexNum << endl;
        #endif

        if(!isReduce){
            Reduce();
            isReduce = true;
        }
        
        #ifdef LOCAL
        puts("-------------------------");
        #endif
        // cout << "reduce_vertex_vis : " << endl;
        int reduce_vertex_vis_num = 0;
        for(int i = 1;i <= ver;i++){
            // cout << i << " " << reduce_vertex_vis[i] << endl;
            if(reduce_vertex_vis[i]){
                reduce_vertex_vis_num++;
            }
        }
        #ifdef LOCAL
        cout << "reduce_vertex_vis_num : " << reduce_vertex_vis_num << endl;
        #endif

        #ifdef LOCAL
        puts("-------------------------");
        cout << "reduceEdges_cnt : " << reduceEdges_cnt << endl;
        for(int i = 1;i <= reduceEdges_cnt;i++){
            for(int j = 0;j < reduceEdges[i].size();j++){
                cout << reduceEdges[i][j] << " ";
            }
            puts("");
        }
        #endif

        #ifdef LOCAL
        puts("-------------------------");
        cout << "after reduce vertex's degree : " << endl;
        #endif
        int afterReduceVertexNum = 0;
        for(int i = 1;i <= ver;i++){
            if(!reduce_vertex_vis[i]){
                if(new_g_vertex_vis[i]){
                    #ifdef LOCAL
                    cout << i << " " << degree_mp[i] << " " << endl;
                    #endif
                    afterReduceVertexNum++;
                }
            }
        }
        #ifdef LOCAL
        cout << "afterReduceVertexNum : " << afterReduceVertexNum << endl;
        #endif

        //reduce g to update_g
        // puts("");
        // puts("-------------------------");
        // cout << "reduce g to update_g : " << endl;
        for(int i = 1;i <= ver;i++){
            for(int j = 1;j <= ver;j++){
                // cout << g[i][j] << " ";
                if(g[i][j]){
                    g_edge++;
                }
                if(g[i][j]){
                    update_g[i][j] = true;
                }
            }
            // puts("");
        }

        #ifdef LOCAL
        puts("-------------------------");
        cout << "after reduce g_edge : " << g_edge / 2 << endl;
        #endif
        if(g_edge == 0){
            #ifdef LOCAL
            cout << "Case: 1 OK!" << endl;
            #endif
            break;
        }

        /**
         * Hard Clause produce
         */
        for(int i = 1; i <= ver; i++) {
            if(!reduce_vertex_vis[i]){
                if(new_g_vertex_vis[i]){
                    for(int j = 1; j <= ver; j++) {
                        if(!reduce_vertex_vis[j]){
                            if(new_g_vertex_vis[j]){
                                if(i == j) {
                                    continue;
                                } else if(i < j) {
                                    if(!g[i][j] && !g[j][i]) {
                                        hard_clause_vertex.push_back(i);
                                        hard_clause_vertex.push_back(j);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        #ifdef LOCAL
        puts("-------------------------");
        #endif
        // cout << "Vertex produce hard clause : ";
        // for(int i = 0; i < hard_clause_vertex.size(); i++) {
        //     cout << hard_clause_vertex[i] << " ";
        // }
        // puts("");
        // cout << "Hard Clause :" << endl;
        for(int i = 0; i < hard_clause_vertex.size();) {
            // cout << "-" << hard_clause_vertex[i] << " -" << hard_clause_vertex[i + 1] << endl;

            int temp = hard_clause_vertex[i];
            int temp2 = hard_clause_vertex[i + 1];

            hard_clause_martix_graph_temp[temp][temp2] = true;
            hard_clause_martix_graph_temp[temp2][temp] = true;

            i += 2;
            hard_count++;
        }
        #ifdef LOCAL
        cout << "hard_count : " << hard_count << endl;
        #endif
        
        //hard_clause_martix_graph_temp
        for(int i = 1;i <= ver;i++){
            for(int j = 1;j <= ver;j++){
                int temp = hard_clause_martix_graph_temp[i][j];
                if(temp == true){
                    hard_clause_martix_graph_vector[i].push_back(j);
                } 
            }
        }

        // puts("-------------------------");
        // cout << "hard_clause_martix_graph_vector : " << endl;
        // for(int i = 1; i <= ver; i++) {
        //     if(!reduce_vertex_vis[i]){
        //         if(new_g_vertex_vis[i]){
        //             cout << i << " ";
        //             for(int j = 0; j < hard_clause_martix_graph_vector[i].size(); j++) {
        //                 cout << hard_clause_martix_graph_vector[i][j] << " ";
        //             }
        //             puts("");
        //         }
        //     }
        // }

        /**
         * Soft Clause produce
         */
        for(int i = 1;i <= ver;i++){
            if(!reduce_vertex_vis[i]){
                if(new_g_vertex_vis[i]){
                    soft_clause_vertex.push_back(i);
                }
            }
        }
        // puts("-------------------------");
        // cout << "Vertex produce soft clause : ";
        // for(int i = 0; i < soft_clause_vertex.size(); i++) {
        //     cout << soft_clause_vertex[i] << " ";
        // }
        // puts("");

        int independent_count  = IndependentSet();
        #ifdef LOCAL
        puts("-------------------------");
        cout << "independent_count : " << independent_count << endl;
        #endif

        #ifdef LOCAL
        puts("-------------------------");
        cout << "Soft Clause : " << endl;
        #endif
        for(int i = 0; i < independent_count; i++) { 
            vector<ll> independentSet_temp;
            for(int j = 0; j < independentSet[i].size(); j++) {
                independentSet_temp.push_back(independentSet[i][j]);
            }
            for(int k = 0;k < independentSet_temp.size();k++){
                #ifdef LOCAL
                cout << independentSet_temp[k] << " ";
                #endif
                int temp = independentSet_temp[k];
                soft_clause_set[soft_count].insert(temp);
            }
            #ifdef LOCAL
            puts("");
            #endif
            soft_count++;
            independentSet_temp.clear();
        }
        #ifdef LOCAL
        cout << "soft_count : " << soft_count << endl;
        #endif

        #ifdef LOCAL
        puts("-------------------------");
        #endif
        int incongruentClauseSetUpperBound = IncongruentClauseSetUpperBound(independent_count);

        #ifdef LOCAL
        puts("-------------------------");
        cout << "incongruentClauseSetUpperBound : " << incongruentClauseSetUpperBound << endl;
        #endif 

        #ifdef LOCAL
        puts("-------------------------");
        #endif
        int bound_temp = independent_count;
        int bound = ceil( (bound_temp + 3) / 2 );
        #ifdef LOCAL
        cout << "bound : " << bound << endl;
        #endif

        Prune(bound);

        // puts("-------------------------");
        // cout << "update_g : " << endl;
        for(int i = 1;i <= ver;i++){
            for(int j = 1;j <= ver;j++){
                // cout << update_g[i][j] << " ";
                if(update_g[i][j]){
                    update_g_edge++;
                }
            }
            // puts("");
        }

        #ifdef LOCAL
        puts("-------------------------");
        cout << "update_g_edge : " << update_g_edge / 2 << endl;
        #endif
        if(update_g_edge == 0){
            #ifdef LOCAL
            cout << "Case: 2 OK!" << endl;
            #endif
            break;
        }
        
        #ifdef LOCAL
        puts("-------------------------");
        cout << "end vertex's degree : " << endl;
        #endif
        int endVertexNum = 0;
        for(int i = 1;i <= ver;i++){
            if(!reduce_vertex_vis[i]){
                if(new_g_vertex_vis[i]){
                    if(degree_mp[i]){
                        #ifdef LOCAL
                        cout << i << " " << degree_mp[i] << " " << endl;
                        #endif
                        endVertexNum++;
                        retain_n.push_back(i);
                    }
                }
            }
        }
        #ifdef LOCAL
        cout << "endVertexNum : " << endVertexNum << endl;
        #endif

        #ifdef LOCAL
        puts("-------------------------");
        cout << "deleteVertexByDegree_set : ";
        #endif
        for(it = deleteVertexByDegree_set.begin(); it != deleteVertexByDegree_set.end(); ++it) {
            #ifdef LOCAL
            printf("%d ", *it);
            #endif
            deleteVertexAndNeighbor[*it] = true;
        }
        #ifdef LOCAL
        puts("");
        cout << "retain_n : ";
        for(int i = 0;i < retain_n.size();i++){
            cout << retain_n[i] << " ";
        }
        #endif

        #ifdef LOCAL
        puts("");
        #endif
        // puts("-------------------------");
        for(int i = 1;i <= ver;i++){
            if(deleteVertexAndNeighbor[i]){
                for(int j = 1;j <= ver;j++){
                    if(deleteVertexAndNeighbor[j]){
                        if(g[i][j]){
                            new_g[i][j] = new_g[j][i] = true;
                        }
                    }
                }
            }
        }
        // cout << "G' : " << endl;
        for(int i = 1;i <= ver;i++){
            for(int j = 1;j <= ver;j++){
                if(i < j){
                    if(new_g[i][j]){
                        newEdge++;
                    }
                }
                // cout << new_g[i][j] << " ";
            }
            // puts("");
        }

        #ifdef LOCAL
        puts("-------------------------");
        cout << "newEdge : " << newEdge << endl;
        #endif

        #ifdef LOCAL
        puts("-------------------------");
        #endif 
        double temp = retain_n.size() / (bound - 1);
        #ifdef LOCAL
        cout << "deleteVertexByDegree_set.size : " << deleteVertexByDegree_set.size() << endl;
        cout << "retain_n.size : " << retain_n.size() << endl;
        cout << "bound : " << bound << endl; 
        cout << "retain_n.size / (bound - 1) : " << temp << endl;
        #endif
        double temp_s = pow( temp, (bound - 2));
        S = abs(floor(temp_s));
        // S = 7;
        if(floor(temp_s) > 1e6){
            S = 1e6;
        }
        #ifdef LOCAL
        cout << "S : " << S << endl;
        #endif

        /*
         *File to python
         */
        fileCnt++;
        ofstream ofs1;
        string filePath = "input_data/";
        string updateFileName_py = fileName + "_py_" + to_string(fileCnt) + ".txt";
        ofs1.open(filePath + updateFileName_py, ios::out);
        for(int i = 1;i <= ver;i++){ 
            for(int j = 1;j <= ver;j++){
                if(i < j){
                    if(update_g[i][j]){
                        ofs1 << i << " " << j << endl;
                    }
                }
            }
        }
        ofs1.close();

        #ifdef LOCAL
        puts("-------------------------");
        #endif 
        string ss_k = to_string(_k);
        string ss_m = "0";
        string ss_python = "python3 kplexes.py --k=" + ss_k + " --m=" + ss_m + " --fileName=" + filePath + updateFileName_py;
        string ss_python_cmd = ss_python + " > ans/" + fileName + "_py_" + to_string(fileCnt) + "_ans.txt";
        #ifdef LOCAL
        cout << ss_python_cmd << endl;
        #endif

        #ifdef LOCAL
        cout << "start enumerate..." << endl;
        #endif 
        system(ss_python_cmd.c_str());  
        
        #ifdef LOCAL
        puts("-------------------------");
        #endif
        string kplexNumFilePath = "ans/" + fileName + "_py_" + to_string(fileCnt) + "_ans.txt";
        // string kplexNumFilePath = "ans/P10_py_1_ans.txt";
        int kplexNum = GetKplexNum(kplexNumFilePath) / 3;
        // int kplexNum = 8;
        #ifdef LOCAL
        cout << "now kplexNum : " << kplexNum << endl;
        #endif 
        GetKplexNumResults(kplexNumFilePath, kplexNum);

        #ifdef LOCAL
        puts("-------------------------");
        cout << "_k : " << _k << endl;
        #endif

        if(S >= _k){
            #ifdef LOCAL
            cout << "Case: 3 OK!" << endl;
            #endif 
            break;
        }else{
            if(kplexNum >= _k){
                #ifdef LOCAL
                cout << "Case: 4 OK!" << endl;
                #endif 
                break;
            }else{
                /*
                *File to c++
                */
                #ifdef LOCAL
                cout << "Case: 5 OK!" << endl;
                #endif 
                ofstream ofs2;
                string updateFileName_c = "input_data_c/" + fileName + "_c_" + to_string(fileCnt) + ".clq";
                ofs2.open(updateFileName_c, ios::out);
                // ofs2 << "p edge " << deleteVertexByDegree_set.size() << " " << newEdge << endl;
                ofs2 << "p edge " << ver << " " << newEdge << endl;
                for(int i = 1;i <= ver;i++){ 
                    for(int j = 1;j <= ver;j++){
                        if(i < j){
                            if(new_g[i][j]){
                                ofs2 << "e " << i << " " << j << endl;
                            }
                        }
                    }
                }
                ofs2.close();
                _k = _k - kplexNum;
                strFilePath = updateFileName_c;
            }
        }
        // break;
    }

    #ifdef LOCAL
    puts("-------------------------");
    cout << "allKplexNum : " << allNum << endl;
    cout << "K : " << K << endl;
    #endif 
    int temp_K;
    if(allNum <= K){
        temp_K = allNum;
    }else{
        temp_K = K;
    }
    for(int i = 1;i <= temp_K;i++){
        int size = kplexNumResults_vector[i].size();
        mp_kplex_size[i] = size;
        for(int j = 0;j < kplexNumResults_vector[i].size();j++){
            cout << kplexNumResults_vector[i][j] << " ";
        }
        puts("");
    }
    // for(int i = 1; i <= temp_K; i++) {
	// 	cout << i << " " << mp_kplex_size[i] << endl;
	// }
    vector< pair<int, int> > vec_kplex_size(mp_kplex_size.begin(), mp_kplex_size.end());
	sort(vec_kplex_size.begin(), vec_kplex_size.end(), cmp);
    puts("");
	for(int i = 1; i <= temp_K; i++) {
		int temp = vec_kplex_size[i - 1].first;
		for(int j = 0; j < kplexNumResults_vector[temp].size(); j++) {
			cout << kplexNumResults_vector[temp][j] << " ";
		}
		puts("");
	}

    if(allNum < K){
        int temp = K - allNum;
        for(int i = 1;i <= temp;i++){
            for(int j = 0;j < reduceEdges[i].size();j++){
                cout << reduceEdges[i][j] << " ";
            }
            puts("");
        }
    }
    return 0;
}
