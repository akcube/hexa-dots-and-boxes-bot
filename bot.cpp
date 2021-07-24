#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

#define endl "\n"

const int GRID_SIZE = 6;

const int dx[6] = { 1, 1, 1, -1, -1, -1};
const int dy[6] = {-1, 0, 1,  1,  0, -1};

class Hexagon{
public:
	int x, y;
	vector<int> edges;
	int hex_count;
	Hexagon(){
		edges.resize(6);
		hex_count = 0;
	}
	void addEdge(int idx){
		if(edges[idx]==0) hex_count++;
		edges[idx] = 1;
	}
	void removeEdge(int idx){
		if(edges[idx]!=0) hex_count--;
		edges[idx] = 0;
	}
};

vector<vector<Hexagon>> grid;

map<vector<int>, vector<int>> bijection;

void take_input(){
	grid.resize(GRID_SIZE);
	for(auto &row:grid)
		row.resize(GRID_SIZE);

	int next_move;
	cin>>next_move;
	int num_mapping;
	cin>>num_mapping;

	for(int i=0; i<num_mapping; i++){
		vector<int> map_one(3);
		vector<int> map_two(3);
		
		for(auto &j:map_one) cin>>j;
		for(auto &j:map_two) cin>>j;

		// swap(map_one[0], map_one[1]);
		// swap(map_two[0], map_two[1]);
		bijection[map_one] = map_two;
	}

	for(int x=0; x<GRID_SIZE; x++){
		for(int y=0; y<GRID_SIZE; y++){
			grid[x][y].x = x;
			grid[x][y].y = y;
			for(int i=0; i<6; i++){
				int e;
				cin>>e;	
				if(e!=0)
					grid[x][y].addEdge(i);
			}
		}
	}
}

int even_dx[6] = { 0, 1, 0, -1, -1, -1};
int even_dy[6] = {-1, 0, 1,  1,  0, -1};
int  odd_dx[6] = { 1, 1, 1, 0, -1,  0};
int  odd_dy[6] = {-1, 0, 1, 1,  0, -1};

int checkGoodHex(Hexagon hex){
	int x=hex.x;
	int y=hex.y;

	vector<int> good_edges;
	for(int i=0; i<6; i++){
		if(hex.edges[i]!=0) continue;
		if(bijection.find({x, y, i})==bijection.end()){
			good_edges.push_back(i);
		}
		else{
			vector<int> mapped(3);
			mapped = bijection[{x, y, i}];
			int h = grid[mapped[0]][mapped[1]].hex_count;
			if(h!=4) good_edges.push_back(i);
		}
	}
	if(!good_edges.empty()) 
		return good_edges[rand()%good_edges.size()];
	
	return -1;
}

int dfs(int x, int y, int k){
	if(grid[x][y].hex_count<5)
		return k;
	if(grid[x][y].hex_count==6){
		return k+1;
	}
	int e;
	vector<int> mapped;
	int idx=0;
	for(auto edge:grid[x][y].edges){
		if(edge==0){
			e = idx;
			grid[x][y].addEdge(e);
			if(bijection.find({x, y, e})==bijection.end()){
				return k+1;
			}
			mapped = bijection[{x, y, e}];
			grid[mapped[0]][mapped[1]].addEdge(mapped[2]);
		}
		idx++;
	}

	int len = dfs(mapped[0], mapped[1], k+1);
	grid[mapped[0]][mapped[1]].removeEdge(mapped[2]);
	grid[x][y].removeEdge(e);
	return len;
}

int main(void){		
	take_input();
	srand(time(0));
	vector<vector<Hexagon>> edge_ct(7);

	for(auto row:grid){
		for(auto cell:row){
			edge_ct[cell.hex_count].push_back(cell);
		}
	}	

	if(edge_ct[5].empty()){
		vector<pair<Hexagon, int>> possiblities;
		for(int i=0; i<=3; i++){
			//if(i==2) continue;
			for(auto cell:edge_ct[i]) {
				int x = checkGoodHex(cell);
				if(x!=-1)
					possiblities.push_back({cell, x});
			}
		}
		//have to make 4 hex move
		if(possiblities.empty()){
			pair<Hexagon, int> best;
			int mi = INT_MAX;
			for(auto cell:edge_ct[4]){
				int idx=0;
				for(auto edge:cell.edges){
					if(edge==0){
						int e = idx;
						grid[cell.x][cell.y].addEdge(e);
						vector<int> mapped;
						bool d_upd = false;
						if(bijection.find({cell.x, cell.y, e})!=bijection.end()){
						 	mapped = bijection[{cell.x, cell.y, e}];
							grid[mapped[0]][mapped[1]].addEdge(mapped[2]);
							d_upd = true;
						}
						int len = dfs(cell.x, cell.y, 0);
						int len2 = 0;
						//cout<<len<<endl;
						if(d_upd){
							len2 = dfs(mapped[0], mapped[1], 0);
							grid[mapped[0]][mapped[1]].removeEdge(mapped[2]);
						}
						grid[cell.x][cell.y].removeEdge(e);
						mi = min(len+len2, mi);
					}
					idx++;
				}

			}
			vector<pair<Hexagon, int>> good;
			for(auto cell:edge_ct[4]){
				int idx=0;
				for(auto edge:cell.edges){
					if(edge==0){
						int e = idx;
						grid[cell.x][cell.y].addEdge(e);
						vector<int> mapped;
						bool d_upd = false;
						if(bijection.find({cell.x, cell.y, e})!=bijection.end()){
						 	mapped = bijection[{cell.x, cell.y, e}];
							grid[mapped[0]][mapped[1]].addEdge(mapped[2]);
							d_upd = true;
						}
						int len = dfs(cell.x, cell.y, 0);
						int len2 = 0;
						//cout<<len<<endl;
						if(d_upd){
							len2 = dfs(mapped[0], mapped[1], 0);
							grid[mapped[0]][mapped[1]].removeEdge(mapped[2]);
						}
						grid[cell.x][cell.y].removeEdge(e);
						if(len+len2==mi){
							good.push_back({cell, e});
						}
					}
					idx++;
				}
				
			}
			//assert(grid[best.first.x][best.first.y].edges[best.second]==0);
			pair<Hexagon, int> chosen = good[rand()%good.size()];
			cout<<chosen.first.x<<" "<<chosen.first.y<<" "<<chosen.second<<endl;
			//cout<<"worst"<<endl;
			return 0;
		}
		else{
			pair<Hexagon, int> chosen = possiblities[rand()%possiblities.size()];
			assert(grid[chosen.first.x][chosen.first.y].edges[chosen.second]==0);
			cout<<chosen.first.x<<" "<<chosen.first.y<<" "<<chosen.second<<endl;
			//cout<<"rando"<<endl;
			return 0;
		}
	}
	else{
		int ma = 0;
		vector<Hexagon> good_hexagons;
		for(auto cell:edge_ct[5]){
			int len = dfs(cell.x, cell.y, 0);
			ma = max(len, ma);
		}
		for(auto cell:edge_ct[5]){
			int len = dfs(cell.x, cell.y, 0);
			if(len==ma)
				good_hexagons.push_back(cell);
		}
		Hexagon chosen_hexagon = good_hexagons[rand()%good_hexagons.size()];
		int e = -1;
		int idx = 0;
		for(auto edge:chosen_hexagon.edges){
			if(edge==0) e = idx;
			idx++;
		}
		pair<Hexagon, int> chosen = {chosen_hexagon, e};
		//assert(grid[chosen.first.x][chosen.first.y].edges[chosen.second]==0);
		cout<<chosen.first.x<<" "<<chosen.first.y<<" "<<e<<endl;
		//cout<<"best"<<endl;
		return 0;
	}

}