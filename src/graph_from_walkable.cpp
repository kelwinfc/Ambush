#include "graph_from_walkable.hpp"
#include <clocale>

static int counter = 0;

class vertex {
    public:
        int index;
        vector< pair<int, int> > boundaries;
        vector< pair<int, int> > points;
        int area;
        vector<int> adj;
        pair<int, int> cntr;
        int n;
        
        vertex(){
            index = counter++;
            area = 0;
            n = 0;
        }
        
        void add_point(int r, int c, bool is_boundary=true){
            
            if ( is_boundary ){
                boundaries.push_back( make_pair(r,c) );
            }
            points.push_back( make_pair(r, c) );
            
            cntr.first += r;
            cntr.second += c;
            
            area++;
            n++;
        }
        
        void remove(){
            counter--;
        }
        
        pair<int, int> center(){
            pair<int, int> ret = cntr;
            
            if ( n == 0 ){
                return ret;
            }
            
            ret.first /= n;
            ret.second /= n;
            
            return ret;
        }
};

bool is_boundary(Mat& img, int r, int c)
{
    for ( int dr = -1; dr < 2; dr++ ){
        for ( int dc = -1; dc < 2; dc++ ){
            if ( dr == 0 && dc == 0)
                continue;
            
            int nr = r + dr;
            int nc = c + dc;
            
            if ( 0 <= nr && nr < img.rows &&
                 0 <= nc && nc < img.cols &&
                 img.at<uchar>(nr, nc) != 0)
            {
                return true;
            }
        }
    }
    return false;
}

void get_vertex(Mat& src, vector<vertex>& points)
{
    Mat aux, dilated;
    src.copyTo(aux);
    dilate(aux, dilated, Mat());
    dilate(aux, aux, Mat());
    
    for ( int r = 0; r < src.rows; r++ ){
        for ( int c = 0; c < src.cols; c++ ){
            if ( aux.at<uchar>(r, c) == 0 ){
                vertex v;
                
                stack< pair<int, int> > s;
                s.push( make_pair(r, c) );
                
                while ( !s.empty() ){
                    pair<int, int> next = s.top();
                    s.pop();
                    
                    aux.at<uchar>(next.first, next.second) = 255;
                    
                    bool is_b = is_boundary(dilated, next.first, next.second);
                    v.add_point(next.first, next.second, is_b);
                    
                    for ( int dr = -1; dr < 2; dr++ ){
                        for ( int dc = -1; dc < 2; dc++ ){
                            if ( dr == 0 && dc == 0)
                                continue;
                            
                            int nr = next.first + dr;
                            int nc = next.second + dc;
                            
                            if ( 0 <= nr && nr < aux.rows &&
                                 0 <= nc && nc < aux.cols &&
                                 aux.at<uchar>(nr, nc) == 0)
                            {
                                s.push(make_pair(nr, nc));
                                aux.at<uchar>(nr, nc) = 255;
                            }
                        }
                    }
                }
                
                if ( v.area < 10.0 ){
                    v.remove();
                } else {
                    points.push_back(v);
                }
            }
        }
    }
    
}

void draw_vertex(Mat& img, vector<vertex>& v, Mat& dst)
{
    vector<Mat> channels;
    channels.push_back(img);
    channels.push_back(img);
    channels.push_back(img);
    merge(channels, dst);
    
    for ( uint i=0; i<v.size(); i++){
        Vec3b color(rand() % 155 + 100,
                    rand() % 155 + 100,
                    rand() % 155 + 100);
        
        for ( uint j=0; j<v[i].points.size(); j++ ){
            dst.at<Vec3b>(v[i].points[j].first,
                          v[i].points[j].second) = color;
        }
    }
}

float dist(pair<int,int> a, pair<int,int> b)
{
    float df = a.first - b.first;
    float ds = a.second - b.second;
    
    return sqrt(df * df + ds * ds );
}

bool are_neighbors(vertex& a, vertex& b)
{
    bool ret = 0;
    int max_ret = 1;
    
    for ( uint i = 0; i < a.boundaries.size() && ret < max_ret; i++ ){
        for ( uint j = 0; j < b.boundaries.size() && ret < max_ret; j++ ){
            if ( dist(a.boundaries[i], b.boundaries[j]) < 5 ){
                ret++;
                continue;
            }
        }
    }
    
    return (ret >= max_ret);
}

void get_adj(Mat& img, vector<vertex>& v)
{
    Mat aux;
    draw_vertex(img, v, aux);
    
    for ( uint i = 0; i < v.size(); i++ ){
        for ( uint j = i+1; j < v.size(); j++ ){
            
            if ( are_neighbors(v[i], v[j]) ){
                v[i].adj.push_back(j);
                
                pair<int, int> ci = v[i].center();
                pair<int, int> cj = v[j].center();
                
                circle(aux, Point(ci.second, ci.first),
                       4, Scalar(0, 0, 255), -1, 8, 0);
                circle(aux, Point(cj.second, cj.first),
                       4, Scalar(0, 0, 255), -1, 8, 0);
                line(aux,
                     Point(ci.second, ci.first),
                     Point(cj.second, cj.first),
                     Scalar(0, 0, 255), 1, 8, 0);
                
                v[i].adj.push_back(j);
            }
        }
    }
    
    imshow("aux", aux);
}

void to_json(vector<vertex>& points, string filename)
{
    FILE * pFile = fopen(filename.c_str(), "w");
    rapidjson::Document d;
    d.SetObject();
    
    d.AddMember("name", "ECM", d.GetAllocator());
    d.AddMember("num_nodes", points.size(), d.GetAllocator());
    d.AddMember("directed", false, d.GetAllocator());
    
    rapidjson::Value nodes(rapidjson::kArrayType);
    for ( uint i = 0; i < points.size(); i++ ){
        rapidjson::Value args(rapidjson::kArrayType);
        
        pair<int, int> center = points[i].center();
        
        rapidjson::Value argx(rapidjson::kObjectType);
        argx.AddMember("arg", "x", d.GetAllocator());
        argx.AddMember("value", center.first, d.GetAllocator());
        args.PushBack(argx, d.GetAllocator());
        
        rapidjson::Value argy(rapidjson::kObjectType);
        argy.AddMember("arg", "y", d.GetAllocator());
        argy.AddMember("value", center.second, d.GetAllocator());
        args.PushBack(argy, d.GetAllocator());
        
        rapidjson::Value area(rapidjson::kObjectType);
        area.AddMember("arg", "area", d.GetAllocator());
        area.AddMember("value", points[i].area, d.GetAllocator());
        args.PushBack(area, d.GetAllocator());
        
        nodes.PushBack(args, d.GetAllocator());
    }
    d.AddMember("nodes", nodes, d.GetAllocator());
    
    rapidjson::Value edges(rapidjson::kArrayType);
    for ( uint i = 0; i < points.size(); i++ ){
        for ( uint j = 0; j < points[i].adj.size(); j++ ){
            int from = points[i].index;
            int to = points[points[i].adj[j]].index;
            double cost = dist(points[from].center(), points[to].center());
            
            rapidjson::Value e(rapidjson::kObjectType);
            e.AddMember("src", from, d.GetAllocator());
            e.AddMember("dst", to, d.GetAllocator());
            e.AddMember("cost", cost, d.GetAllocator());
            
            rapidjson::Value args(rapidjson::kObjectType);
            args.SetObject();
            
            e.AddMember("args", args,
                        d.GetAllocator());
            
            edges.PushBack(e, d.GetAllocator());
        }
    }
    d.AddMember("edges", edges, d.GetAllocator());
    
    rapidjson::FileStream f(pFile);
    rapidjson::Writer<rapidjson::FileStream> writer(f);
    d.Accept(writer);
    
}

int main(int argc, char* argv[])
{
    srand (time(NULL));
    srand (time(NULL));
    setlocale(LC_NUMERIC, "POSIX");
    
    argc--;
    argv++;
    
    Mat src = imread(argv[0], 0);
    threshold(src, src, 100, 255, 0);
    
    imshow("src", src);
    
    vector<vertex> p;
    get_vertex(src, p);
    
    Mat drawing;
    draw_vertex(src, p, drawing);
    
    imshow("vertex", drawing);
    
    get_adj(src, p);
    waitKey(0);
    
    to_json(p, argv[1]);
    
    cout << "Bye!" << endl;
    return 0;
}
