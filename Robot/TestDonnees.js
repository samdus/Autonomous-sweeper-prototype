var stats = {
    stabilite: "",
    nbEchantillionTotal: 0,
    nbErreurTotal: 0,
    nbVraiErreur: 0,
    casGaucheDroite: {nb: 0, cas: []},
    casRien: {nb: 0, cas: []},
    casMauvaisCote: {nb: 0, cas: []},
    casFauxRien: {nb: 0, cas: []}
};
var derniereHeure;

db.getCollection('consoleContainer')
.find(
    { 
        createdAt: { 
            $gte: ISODate("2016-11-03T21:08:51.943Z").getTime(),
            $lte: ISODate("2016-11-04T00:20:12.645Z").getTime()
        },
        messageLevel: {
            $eq: 'info'
        }
    },
    {
        messageValue: 1,
        createdAt: 1
    })
.sort(
    {
        createdAt: 1
    }
).forEach( function(doc) {
    doc.createdAt = new Date(doc.createdAt);
    
    var message = doc.messageValue.split(" ");
    var mouvement = message[2].split("");
    var tempEcoule = doc.createdAt - derniereHeure;
    
    doc.action = {
        depart:  message[3],
        arrive:  message[4],
        diff:    "" + (message[3] - message[4]),
        debut:   derniereHeure.getHours() + ":" + derniereHeure.getMinutes() + ":" + derniereHeure.getSeconds() + ":" + derniereHeure.getMilliseconds(),
        fin:     doc.createdAt.getHours() + ":" + doc.createdAt.getMinutes() + ":" + doc.createdAt.getSeconds() + ":" + doc.createdAt.getMilliseconds(),
        ecoule:  tempEcoule + " ms",
        suppose: (mouvement[0] == 0 ? "Rien" : (mouvement[0] == 1 ? "Gauche" : (mouvement[0] == 2 ? "Droite" : "??????"))),
        fait:    (mouvement[1] == 0 ? "Rien" : (mouvement[1] == 1 ? "Gauche" : (mouvement[1] == 2 ? "Droite" : "Rien")))
    };
    
    if(doc.action.suppose != doc.action.fait)
    {
        printjson(doc);
        stats.nbErreurTotal++;
        
        if((doc.action.suppose == "Rien" || doc.action.fait == "Rien") && (doc.action.diff <= 3 && doc.action.diff >= -3))
        {
            stats.casRien.nb++;
            stats.casRien.cas.push(doc.action);
        }
        else if(doc.action.suppose == "Rien" || doc.action.fait == "Rien")
        {
            stats.casFauxRien.nb++;
            stats.casFauxRien.cas.push(doc.action);
        }
        else if(    (doc.action.diff < 0 && (doc.action.diff > -174 || doc.action.diff < -184)) 
                 || (doc.action.diff > 0 && (doc.action.diff < 174 || doc.action.diff > 184)))
        {
            stats.casMauvaisCote.nb++;
            stats.casMauvaisCote.cas.push(doc.action);
        }
        else
        {
            stats.casGaucheDroite.nb++;
            stats.casGaucheDroite.cas.push(doc.action);
        }
    }
    
    stats.nbEchantillionTotal++;
    derniereHeure = doc.createdAt;
});

stats.nbVraiErreur = stats.casMauvaisCote.nb + stats.casFauxRien.nb;
stats.stabilite = ((1 - (stats.nbVraiErreur / stats.nbEchantillionTotal)) * 100) + " %";
printjson(stats);

db.StatistiquesRotation.insert({ genereLe: new Date(), stats: stats});