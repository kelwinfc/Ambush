#include "get_walkable_regions.hpp"

inline bool in_interval(int min_v, int v, int max_v)
{
    if ( min_v <= v && v < max_v ){
        return true;
    }
    return false;
}

int get_neighbor(Mat& img, int r, int c, int dr, int dc){
    int nr = r + dr;
    int nc = c + dc;
    
    if ( in_interval(0, nr, img.rows) && in_interval(0, nc, img.cols) ){
        return (img.at<uchar>(nr, nc) != 0 ? 1 : 0);
    } else {
        return -1;
    }
}

int count_neighbors(Mat& img, int r, int c)
{
    int ret = 0;
    
    /*
    -1,-1 -1,+0, -1,+1
    +0,-1 +0,+0, +0,+1
    +1,-1 +1,+0, +1,+1
    */
    int xneighbors[8] = {-1,+0,+1,+1,+1,+0,-1,-1};
    int yneighbors[8] = {-1,-1,-1,+0,+1,+1,+1,+0};
    
    for ( int n = 0; n < 8; n++ ){
        int prev_n = get_neighbor(img, r, c, xneighbors[n], yneighbors[n]);
        int next_n = get_neighbor(img, r, c, xneighbors[(n + 1) % 8],
                                             yneighbors[(n + 1) % 8]
                                 );
        
        if ( prev_n < 0 || next_n < 0 ){
            continue;
        }
        
        if ( !prev_n && next_n ){
            ret++;
        }
    }
    
    return ret;
}

void get_branching_points(Mat& skeleton, vector< pair<int, int> >& points)
{
    points.clear();
    
    for ( int r = 0; r < skeleton.rows; r++ ){
        for ( int c = 0; c < skeleton.cols; c++ ){
            if ( skeleton.at<uchar>(r,c) != 0 && 
                 count_neighbors(skeleton, r, c) > 2 )
            {
                points.push_back(make_pair(r, c));
            }
        }
    }
}

void draw_branching_points(Mat& src, vector< pair<int, int> >& points,
                           Mat& dst)
{
    vector<Mat> channels;
    
    for ( uint i=0; i<3; i++)
        channels.push_back(src);
    merge(channels, dst);
    
    for ( uint k = 0; k < points.size(); k++ ){
        circle(dst, Point(points[k].second, points[k].first),
                          4, Scalar(rand() % 155 + 100,
                                    rand() % 155 + 100,
                                    rand() % 155 + 100), -1, 8, 0);
    }
}

bool is_in_edge(Mat& img, int r, int c)
{
    for ( int dr = -1; dr < 2; dr++ ){
        for ( int dc = -1; dc < 2; dc++ ){
            if ( dr == 0 && dc == 0 ){
                continue;
            }
            
            int nr = r + dr;
            int nc = c + dc;
            
            if ( !in_interval(0, nr, img.rows) ){
                return true;
            }
            
            if ( !in_interval(0, nc, img.cols) ){
                return true;
            }
            
            if ( img.at<uchar>(nr, nc) == 0)
            {
                return true;
            }
        }
    }
    
    return false;
}

void traverse(Mat& obs, Mat& img, int r, int c,
              vector< pair<int, int> >& points)
{
    stack< pair<int, int> > s;
    s.push( make_pair(r, c) );
    img.at<uchar>(r, c) = 0;
    
    while ( !s.empty() ){
        pair<int, int> next = s.top();
        s.pop();
        
        if ( is_in_edge(obs, next.first, next.second) )
            points.push_back(next);
        
        for ( int dr = -1; dr < 2; dr++ ){
            for ( int dc = -1; dc < 2; dc++ ){
                if ( dr == 0 && dc == 0 ){
                    continue;
                }
                
                int nr = next.first + dr;
                int nc = next.second + dc;
                
                if ( in_interval(0, nr, img.rows) &&
                     in_interval(0, nc, img.cols) &&
                     img.at<uchar>(nr, nc) != 0
                   )
                {
                   img.at<uchar>(nr, nc) = 0;
                   s.push(make_pair(nr, nc));
                }
            }
        }
    }
}

void get_obstacles(Mat& src, vector< vector< pair<int, int> > >& obstacles)
{
    Mat dst;
    src.copyTo(dst);
    
    // Top
    vector< pair<int, int> > top;
    for ( int c = 0; c < src.cols; c++ ){
        top.push_back( make_pair(0, c) );
        src.at<uchar>(0, c) = 255;
    }
    obstacles.push_back(top);
    
    // Bottom
    vector< pair<int, int> > bottom;
    for ( int c = 0; c < src.cols; c++ ){
        bottom.push_back( make_pair(src.rows - 1, c) );
        src.at<uchar>(src.rows - 1, c) = 255;
    }
    obstacles.push_back(bottom);
    
    // Left
    vector< pair<int, int> > left;
    for ( int r = 0; r < src.rows; r++ ){
        left.push_back( make_pair(r, 0) );
        src.at<uchar>(r, 0) = 255;
    }
    obstacles.push_back(left);
    
    // Right
    vector< pair<int, int> > right;
    for ( int r = 0; r < src.rows; r++ ){
        right.push_back( make_pair(r, src.cols - 1) );
        src.at<uchar>(r, src.cols - 1) = 255;
    }
    obstacles.push_back(right);
    
    for ( int r = 0; r < dst.rows; r++ ){
        for  ( int c = 0; c < dst.cols; c++ ){
            if ( dst.at<uchar>(r, c) != 0 ){
                vector< pair<int, int> > points;
                traverse(src, dst, r, c, points);
                obstacles.push_back(points);
            }
        }
    }
}

void draw_obstacle_points(Mat& src,
                          vector< vector< pair<int, int> > >& obstacles,
                          Mat& dst
                         )
{
    vector<Mat> channels;
    
    for ( uint i=0; i<3; i++)
        channels.push_back(src);
    merge(channels, dst);
    
    for ( uint k = 0; k < obstacles.size(); k++ ){
        
        Vec3b c(0, 0, 255);
        for ( uint p = 0; p < obstacles[k].size(); p++ ){
            dst.at<Vec3b>(obstacles[k][p].first, obstacles[k][p].second) = c;
        }
    }
}

float dist(pair<int, int> a, pair<int, int> b)
{
    float dx = a.first - b.first;
    float dy = a.second - b.second;
    
    return sqrt(dx * dx + dy * dy);
}

int closest_point(pair<int, int> p, vector< pair<int, int> >& seq)
{
    if ( seq.size() == 0 ){
        return -1;
    }
    
    float min_dist = dist(p, seq[0]);
    int closest = 0;
    
    for ( uint i = 1; i < seq.size(); i++ ){
        float next_dist = dist(p, seq[i]);
        
        if ( next_dist < min_dist ){
            closest = i;
            min_dist = next_dist;
        }
    }
    
    return closest;
}

bool is_clear(Mat& img, pair<int, int> a, pair<int, int> b)
{
    pair<int, int> starting_point = a;
    pair<int, int> ending_point = b;
    float k = 5.0;
    
    float angle_a_b = atan2(b.second - a.second, b.first - a.first);
    starting_point.first  += (int)round(k * cos(angle_a_b));
    starting_point.second += (int)round(k * sin(angle_a_b));
    
    float angle_b_a = atan2(a.second - b.second, a.first - b.first);
    ending_point.first += (int)round(k * cos(angle_b_a));
    ending_point.second += (int)round(k * sin(angle_b_a));
    
    Mat aux = Mat::zeros(img.rows, img.cols, CV_8U);
    
    line(aux,
         Point(starting_point.second, starting_point.first),
         Point(ending_point.second, ending_point.first),
         255, 2, 8, 0);
    
    Mat dst = min(img, aux);
    
    if ( sum(dst)[0] != 0 ){
        return false;
    }
    
    return true;
}

void get_walkable_regions(Mat& obstacles_img,
                          Mat& medial_axis_img,
                          vector< vector< pair<int, int> > >& obstacles,
                          vector< pair<int, int> > medial_axis,
                          Mat& dst
                         )
{
    obstacles_img.copyTo(dst);
    //dst += medial_axis_img;
    
    for ( uint i = 0; i < medial_axis.size(); i++ ){
        for ( uint j = 0; j < obstacles.size(); j++ ){
            int k = closest_point(medial_axis[i], obstacles[j]);
            
            if ( k < 0 ){
                continue;
            }
            
            float min_dist = dist(medial_axis[i], obstacles[j][k]);
            
            for ( uint k = 0; k < obstacles[j].size(); k++ ){
                if ( abs(min_dist - 
                         dist(medial_axis[i], obstacles[j][k])) > 10e-6 )
                {
                    continue;
                }
                
                Mat aux;
                obstacles_img.copyTo(aux);
                aux += medial_axis_img;
                
                bool clear = true;
                
                if ( is_clear(aux, medial_axis[i], obstacles[j][k]) ){
                    clear = true;
                } else {
                    clear = false;
                }
                /*
                dst.copyTo(aux);
                aux += medial_axis_img;
                
                vector<Mat> channels;
                channels.push_back(aux);
                channels.push_back(aux);
                channels.push_back(aux);
                merge(channels, aux);
                */
                
                if ( clear ){
                    line(dst,
                         Point(medial_axis[i].second, medial_axis[i].first),
                         Point(obstacles[j][k].second, obstacles[j][k].first),
                         255, 1, 8, 0);
                }
                /*
                if ( clear ){
                    line(aux,
                         Point(medial_axis[i].second, medial_axis[i].first),
                         Point(obstacles[j][k].second, obstacles[j][k].first),
                         Scalar(0, 255, 0), 1, 8, 0);
                } else {
                    line(aux,
                         Point(medial_axis[i].second, medial_axis[i].first),
                         Point(obstacles[j][k].second, obstacles[j][k].first),
                         Scalar(0, 0, 255), 1, 8, 0);
                }
                
                imshow("aux", aux);
                waitKey(0);
                */
            }
        }
    }
}

int main(int argc, char* argv[])
{
    srand (time(NULL));
    argc--;
    argv++;
    
    string path = argv[1];
    
    Mat blueprint = imread(argv[0], 0);
    threshold(blueprint, blueprint, 100, 255, THRESH_BINARY_INV);
    
    stringstream ss;
    string command;
    
    ss << "python utils/get_navigation_mesh.py " << argv[0] << " " << path;
    getline(ss, command);
    
    if ( system(command.c_str()) ){
        exit(-1);
    }
    
    Mat skeleton = imread(path + "skeleton.jpg", 0);
    Mat obstacles = imread(path + "dst.jpg", 0);
    
    Mat dst;
    
    threshold(skeleton, skeleton, 100, 255, THRESH_BINARY);
    threshold(obstacles, obstacles, 100, 255, THRESH_BINARY_INV);
    
    vector< vector< pair<int, int> > > obs;
    get_obstacles(obstacles, obs);
    
    draw_obstacle_points(obstacles, obs, dst);
    imwrite(path + "obstacles.jpg", dst);
    
    vector< pair<int, int> > p;
    get_branching_points(skeleton, p);
    
    cout << p.size() << " points.\n";
    
    draw_branching_points(skeleton, p, dst);
    imwrite(path + "branching_points.jpg", dst);
    
    Mat wr;
    get_walkable_regions(obstacles, skeleton, obs, p, wr);
    imwrite(path + "walkable_regions.jpg", wr);
    
    /*
    cv2.imwrite(path + "src.jpg", ma)
    
    _, ma = cv2.threshold(ma, 100, 255, cv2.THRESH_BINARY)
    cv2.imwrite(path + "dst.jpg", ma)*/
    
    cout << "Bye!\n";
    return 0;
}
