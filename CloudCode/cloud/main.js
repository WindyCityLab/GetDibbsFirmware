
var express = require('express');
var moment = require('moment');
var app = express();

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

function getSunday(d) {
  d = new Date(d);
  var day = d.getDay(),
      diff = d.getDate() - day; // adjust when day is sunday
  return new Date(d.setDate(diff));
}
function daysBetweenDates(d1, d2) {
	return Math.round((d2-d1)/(1000*60*60*24));
}

app.get('/firstDayOfCurrentWeek', function (req,res) {
	res.send(getSunday(new Date));
})

app.get('/daysToFirstSunday', function (req, res) {
	res.send(daysBetweenDates(new Date, getSunday(new Date)));
})
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
  subQuery.equalTo("resourceID", 1);
  var query = new Parse.Query(Reservation);
  query.matchesQuery("resource",subQuery);
	var firstSunday = getSunday(new Date);
	console.log(firstSunday);
  query.greaterThan("date",firstSunday);
	var fortnightAway = new Date(firstSunday.getTime() + 14*24*60*60*1000);
	console.log(fortnightAway);
	query.lessThan("date",fortnightAway);
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
				var theDate = results[i].get("date");
				theDate.setHours(theDate.getHours() - 6);
//				result.push(theDate);
				result.push(results[i].get("user").get("employeeOf").get("red"));
				result.push(results[i].get("user").get("employeeOf").get("green"));
				result.push(results[i].get("user").get("employeeOf").get("blue"));
				var theDay = theDate.getDay()+1; // We assume Sunday to be 1 not 0 like javascript
				var daysFromFirstSunday = daysBetweenDates(firstSunday,theDate);
				console.log(daysFromFirstSunday);
				if (daysFromFirstSunday > 6)
				{
					theDay = theDay + 7;
				}
				result.push(theDay);
				result.push(theDate.getHours() + 6);
				result.push(results[i].get("user").get("employeeOf").get("uniqueID"));
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
