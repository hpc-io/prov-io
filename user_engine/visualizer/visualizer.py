# import rdflib
# from rdflib.extras.external_graph_libs import rdflib_to_networkx_multidigraph
# import networkx as nx
# import matplotlib.pyplot as plt

# _rdf_path = '/home/runzhou/Downloads/prov-io/c/doc/example_prov.ttl'

# g = rdflib.Graph()
# result = g.parse(_rdf_path, format='turtle')

# G = rdflib_to_networkx_multidigraph(result)

# # Plot Networkx instance of RDF Graph
# pos = nx.spring_layout(G, scale=2)
# edge_labels = nx.get_edge_attributes(G, 'r')
# nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
# nx.draw(G, with_labels=True)

# #if not in interactive mode for 
# plt.show()

# <https://github.com/hpc-io/prov-io/blob/master/c/doc/example_prov.ttl>

import requests

# Replace with your provenance file or uri
response = requests.get("http://www.ldf.fi/service/rdf-grapher?rdf=<https://github.com/hpc-io/prov-io/blob/master/c/doc/example_prov.ttl>&from=ttl&to=png")