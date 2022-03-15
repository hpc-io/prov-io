import rdflib

g = rdflib.Graph()

g.parse("/global/homes/r/rzhan/topreco/code/prov/prov.rdf", format = "turtle")

qres = g.query(
    """SELECT ?x ?version ?accuracy
        WHERE { 
            ?x ns1:Name "batch_size"; 
            ns1:Value "5";
            ns1:Version ?version;
            ns1:Quality ?accuracy;
            }"""
       )


for row in qres:
    print("%s \nbatch_size = 5 has been used. \nVersion: %s\nAccuracy: %s\n" % row)
