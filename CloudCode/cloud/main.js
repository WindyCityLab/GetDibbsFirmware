
var Stripe = require('stripe');
var express = require('express');
var moment = require('moment');
var app = express();
Stripe.initialize('sk_test_4zoxCh6615WAFMEp2P95lpHB');

// var apiKey = "bbGEkMOjeHqYKacEuNU2XNksyx2u5muwP12XkAxS";
// app.use(express.bodyParser());
Parse.Cloud.define("chargeCard", function (request,response) {
	console.log("calling charges create");
	Stripe.Charges.create({
		amount : request.params.amount,
		currency : request.params.currency,
		card : request.params.stripeToken
	},
{
	success : function(httpResponse) {
		console.log("purchase made");
		response.success("Purchase made!");
	},
	error : function(httpResponse) {
		response.error();
	}
});
});

app.get('/test', function (req, res) {
	var AUser = Parse.Object.extend("AUser");
	var query = new Parse.Query(AUser);
	query.equalTo("userID",req.param("userID"));
	console.log("Performing Query with userID =" + req.param("userID"));
	query.first( {
		success : function (results) {
			console.log("Successful Query for " + results.get("name"));
		},
		error : function (error) {
			console.log("query error");
		}
	})
});
app.get('/reserve', function (req,res) {
	var query = new Parse.Query(Parse.Object.extend("AUser"));
	query.equalTo("userID",req.param("userID"));
	console.log("Performing Query with userID =" + req.param("userID"));
	query.first( {
		success : function (results)
		{
			console.log("Successful Query for " + results.get("name"));
			var aUser = results.get("user");
			var query2 = new Parse.Query(Parse.Object.extend("Resource"));
			query2.equalTo("resourceID",req.param("resourceID"));
			query2.first( {
				success : function (results2)
				{
					console.log("got resource = " + results2.get("name"));
					var Reservation = Parse.Object.extend("Reservation");
					var reservation = new Reservation();
					reservation.set("user",results);
					reservation.set("resource",results2);
					var theDate = new Date(req.param("year"), req.param("month"), req.param("day"), req.param("hour"),req.param("minute"),0,0);
					reservation.set("date",theDate);
					console.log("date is " + theDate);
					reservation.save(null, {
						success : function (reservation) {
							console.log("successfully saved new reserveration");
						},
						error : function (reservation, error) {
							console.log(error.message);
						}
					});
				},
				error : function (error) {
					console.log(error)
				}
			});
		},
		error : function (error) {
			console.log("Query unsuccessful");
		}
	});
});

app.get('/resourceName', function (req, res) {
	var Resource = Parse.Object.extend("Resource");
	var query = new Parse.Query(Resource);
	query.equalTo("resourceID", req.param("resourceID"));
	query.first({
		success: function (results)
		{
			res.send('"' + results.get("name") + '"');
		},
		error: function (error)
		{
			console.log(error);
			res.error(400).send({error: "Query Failed"});
		}
	})
});

app.get('/epochTime', function (req,res) {
	res.send('"' + JSON.stringify(moment().valueOf()) + '"');
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
  query.include("user");
	query.include("user.employeeOf");
  query.include("resource");
  query.find({
    success: function (results)
    {
      var result = [];
			result.push(results.length);
      for (var i = 0; i < results.length; i++)
      {
				result.push(results[i].get("user").get("employeeOf").get("red"));
				result.push(results[i].get("user").get("employeeOf").get("green"));
				result.push(results[i].get("user").get("employeeOf").get("blue"));
				var theDay = results[i].get("date").getDay();
				if (theDay == 0)
				{
					theDay = 7
				}
				result.push(theDay);
				result.push(results[i].get("date").getHours()-7);
      }
      res.send(JSON.stringify(result));
    },
    error : function (error) {
      console.log(error);
      res.status(400).send( {error : "Query Failed"});
    }
  })
});

app.listen();
