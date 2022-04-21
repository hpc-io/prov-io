import rdflib

g = rdflib.Graph()

g.parse("/global/homes/r/rzhan/topreco/code/prov/prov.rdf", format = "turtle")

qres = g.query(
    """SELECT ?x ?type ?name ?value ?version ?accuracy
        WHERE { 
            ?x ns1:Name ?name; 
            ns1:Type ?type;
            ns1:Value ?value;
            ns1:Version ?version;
            ns1:Quality ?accuracy;
            }
            ORDER BY DESC(?accuracy) LIMIT 15"""
       )


for row in qres:
    print("%s:\n Type: %s \n %s = %s \n Version: %s\n accuracy: %s\n" % row)
