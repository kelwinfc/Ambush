import sys
import json

# Main
args = sys.argv[1:]
option = args[0]

if option == "--help":
    print "You are using a GraphGenerator to the test phase of A*mbush Algorithm." \
          "You can generate 3 types of graphs:\n" \
          " # translate file [file [...]]: Return all the graphs in json format"
elif option == "--version":
    print "Graph Translate version \"0.1\"\n" \
          "Part of A*mbush Test Package\n"
else:
  # Get all the files
  files = args[1:]
  
  # Cycle for each file, to make a new graph file
  for f in files:    
    # Initialize the both files for graph, the oldone and the newone
    name = f.split(".")[0]
    print "Translating "+name+"..."
    tmpf = open(f,"r")
    newf = open(name+".json","w")
    nodes = int(tmpf.readline())
    
    graph = { "name":name, "num_nodes":nodes, "directed":False,
              "nodes":[], "edges":[] }
    
    # Define the Nodes  
    for i in range(nodes):
        info = tmpf.readline().split(" ")
        x = info[0]
        y = info[1][:-1]
        graph["nodes"].append([ {"arg": "x", "value": x},
                                {"arg": "y", "value": y} ])
    
    # Define the Edges  
    tmpf.readline()
    for line in tmpf:
      info = line.split(" ")
      
      src = int(info[0])
      dst = int(info[1])
      cost = float(info[2][:-1])
      graph["edges"].append({"src":src, "dst":dst, "cost":cost })
    
    json.dump(graph, newf, sort_keys=False, indent=1, separators=(',', ': '))
    
    newf.close()
    tmpf.close()
  print "Graphs were translated"