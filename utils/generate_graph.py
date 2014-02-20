import sys
import networkx as nx
import random as rdm
import matplotlib.pyplot as plt
import json
from math import sqrt

# Grid 2D:
def grid2d(n,m):

    def pos_to_index(i,j, n, m):
        return i*m + j
    def index_to_row(i, n, m):
        return i/m
    def index_to_column(i, n, m):
        return i % m
    
    g = nx.Graph()
    
    for i in xrange(n):
        for j in xrange(m):
            for di in [-1,0,1]:
                for dj in [-1,0,1]:
                    ni = i+di
                    nj = j+dj
                    
                    if 0 <= ni and ni < n and \
                       0 <= nj and nj < m and\
                       (di != 0 or dj != 0) :
                       
                        g.add_edge( pos_to_index(i,j,n,m),
                                    pos_to_index(ni,nj,n,m),
                                  )
    
    for idx, node in enumerate(g.nodes()):
        g.node[node]['y'] = index_to_column(idx, n, m)
        g.node[node]['x'] = index_to_row(idx, n, m)
    
    return g

# Dorogovtsev - Mendes graph generator
def dorogovtsev_mendes(n):
    g = nx.Graph()
    g.add_nodes_from([0,1,2])
    g.add_edges_from([(0,1),(1,2),(2,0)])
    for i in range(3,n):
        edges = g.edges()
        rdm.shuffle(edges)
        edge = edges.pop()
        node1 = edge[0]
        node2 = edge[1]
        g.add_node(i)
        g.add_edges_from([(i,node1),(i,node2)])
    return g

def distance(g, v, w):
    dx = g.node[v]["x"] - g.node[w]["x"]
    dy = g.node[v]["y"] - g.node[w]["y"]

    return sqrt(dx**2 + dy**2)

def random_geometric_graph(n,r):    
    g = nx.random_geometric_graph(n,r)
    for node in g.nodes():
        g.node[node]["x"] = g.node[node]["pos"][0]
        g.node[node]["y"] = g.node[node]["pos"][1]
        g.node[node].pop("pos")
    
    for v in g.nodes():
        for w in g.edge[v]:
            g.edge[v][w]['cost'] = distance(g, v, w)
    
    return g

def navigable_small_world_graph(n):
    g_aux = nx.navigable_small_world_graph(n,1,5, 1.0)
    g = nx.Graph()

    index = {}
    
    for idx, v in enumerate(g_aux.nodes()):
        (x,y) = v
        g.add_node(idx)
        g.node[idx]["x"] = x
        g.node[idx]["y"] = y
        index[v] = idx
    
    for v in g_aux.nodes():
        for w in g_aux.edge[v]:
            idv = index[v]
            idw = index[w]
            g.add_edge(idv, idw, cost=1)#distance(g, idv, idw))
    
    return g

def translate_grid(g):
    newg = nx.Graph()
    return newg
        
# Main
args = sys.argv[1:]
option = args[0]

if option == "--help":
    print "You are using a GraphGenerator to the test phase of A*mbush Algorithm." \
          "You can generate that graphs:\n" \
          " # grid2nn name n: Return a 2D grid of size n*n graph\n" \
          " # grid2nm name n m: Return a 2D grid of size n*m graph\n" \
          " # dm name n: Return a Dorogovtsev-Mendes graph\n" \
          " # dgm name n: Return a Dorogovtsev-Goltsev-Mendes graph\n" \
          " # ws name n k p: Return a Watts-Strogatz small-world graph\n" \
          " # rgg name n r [d]: Return a random geometric graph\n" \
          " # gtg name n t: Return a geographical geometrica graph\n" \
          " # wg name n: Return a Waxamn Graph\n" \
          " # nswg name n: Return a Navigable Small Sworld Graph\n"
elif option == "--version":
    print "Graph Generator version \"0.1\"\n" \
          "Part of A*mbush Test Package\n" \
          "The most graph are generated with Networkx Library"
elif len(args) < 3:
    print("generate_graph: Too few arguments")
    print("Try generate_graph --help for more information")
    sys.exit()

else:
    file_name = args[1]
    g = nx.Graph(name=file_name)
    n = int(args[2])
    if option == "gridnn":
        g = nx.grid_2d_graph(n,n)
        name = 'Grid 2D ('+str(n)+'x'+str(n)+')'
    elif option == "gridnm":
        m = int(args[3])
        g = nx.grid_2d_graph(n,m)
        name = 'Grid 2D ('+str(n)+'x'+str(m)+')'
    elif option == "grid2nn":
        g = grid2d(n,n)
        name = 'Grid 2D V2 ('+str(n)+'x'+str(n)+')'
    elif option == "grid2nm":
        m = int(args[3])
        g = grid2d(n,m)
        name = 'Grid 2D V2 ('+str(n)+'x'+str(m)+')'
    elif option == "dm":
        g = dorogovtsev_mendes(n)
        name = 'Dorogovtsev y Mendes ('+str(n)+')'
    elif option == "dgm":
        g = nx.dorogovtsev_goltsev_mendes_graph(n)
        name = 'Dorogovtsev, Goltsev y Mendes ('+str(n)+')'
    elif option == "ws":
        k = int(args[3])
        p = float(args[4])
        g = nx.watts_strogatz_graph(n,k,p)
        name = 'Watts y Strogatz  (n='+str(n)+', k='+str(k)+', p='+str(p)+')'
    elif option == "rgg":
        r = float(args[3])
        name = ""
        if len(args) > 4:
            d = int(args[4])
            g = nx.random_geometric_graph(n,r,d)
            name = 'Random Geometric Graph (n='+str(n)+', r='+str(r)+', d='+str(d)+')'
        else:
            g = random_geometric_graph(n,r)
            name = 'Random Geometric Graph (n='+str(n)+', r='+str(r)+')'
    elif option == "gtg":
        t = float(args[3])
        g = nx.geographical_threshold_graph(n,t)
        name = 'Geographical Threshold Graph (n='+str(n)+', t='+str(t)+')'
    elif option == "wg":
        g = nx.waxman_graph(n)
        name = 'Waxman Graph (n='+str(n)+')'
    elif option == "nswg":
        g = navigable_small_world_graph(n)
        name = 'Navigable Small World Graph (n='+str(n)+')'    
    else:
        print("generate_graph: invalid option:"+option)
        print("Try generate_graph --help for more information")
        sys.exit()
    name += " - "+file_name 

    # Write The File
    newf = open(file_name+".json","w")

    # Define the Graph
    graph = { "name":name, "num_nodes":len(g.nodes()), "directed":False,
              "nodes":[], "edges":[] }

    # Define the Nodes
    for node in g.nodes(data=True):
        info = []
        for data in node[1]:
            info.append({"arg":data, "value":str(node[1][data])})
        graph["nodes"].append(info)

    # Define the Edges
    for edge in g.edges(data=True):
        cost = 1.0
        if len(edge[2]) > 0:
            if ('cost' in edge[2]):
                cost = edge[2].pop('cost',None)
        info = []
        for data in edge[2]:
            info.append({"arg":data, "value":str(edge[2][data])})
        
        src = int(edge[0])
        dst = int(edge[1])
        
        graph["edges"].append({"src":src, "dst":dst, "cost":cost, "args":info })

    json.dump(graph, newf, sort_keys=False, indent=1, separators=(',', ': '))
    newf.close()
