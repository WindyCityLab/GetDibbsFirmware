var express = require('express');
var app = express();

// var apiKey = "bbGEkMOjeHqYKacEuNU2XNksyx2u5muwP12XkAxS";
// app.use(express.bodyParser());

app.get('/test', function (req, res) {
	res.send('Hello World!');
});

function getColor(forClient)
{
  var red = parseInt(forClient.get('red'));
  var green = parseInt(forClient.get('green'));
  var blue = parseInt(forClient.get('blue'));
  var color = (red * (256 ^ 2)) + (green * 256) + blue;
  return color;
}
app.get('/reservations', function (req,res) {
  var Reservation = Parse.Object.extend("Reservation");
  var Resource = Parse.Object.extend("Resource");

  var subQuery = new Parse.Query(Resource);
  subQuery.equalTo("resourceID", req.param("resourceID"));
  var query = new Parse.Query(Reservation);
  query.matchesQuery("resource",subQuery);
  query.greaterThan("date",(new Date));
  query.include("client");
  query.include("resource");
  query.find({
    success: function (results)
    {
      var result = [];
      for (var i = 0; i < results.length; i++)
      {
        var items = {};
        items["col"] = getColor(results[i].get("client"));
//        items["cn"] = results[i].get("client").get("name");
        items["rn"] = results[i].get("resource").get("name");
        items["dow"] = results[i].get("date").getDay();
        items["hr"] = results[i].get("date").getHours()-5;
        result.push(items);
      }
      res.send(JSON.stringify(result));
    },
    error : function (error) {
      console.log(error);
      res.status(400).send( {error : "Query Failed"});
    }
  })
});

app.get('/clients', function (req, res) {
  var Client = Parse.Object.extend('Client');
  var query = new Parse.Query(Client);
  query.select("blue","red","green","name");
  query.equalTo("clientID",req.param('clientID'));
  query.first({
    success: function(results) {
      var red = parseInt(results.get('red'));
      var green = parseInt(results.get('green'));
      var blue = parseInt(results.get('blue'));
      var color = (red * (256 ^ 2)) + (green * 256) + blue;
      res.send('{name=' + results.get('name') + ', color = ' + color + '}');
      // res.send(results);
    },
    error : function (error) {
      res.status(400).send( {error: "Query Failed"});
    }
  });
});

app.listen();
