//
//  Reservation.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 9/12/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//
enum ReservationAction
{
    case Created
    case Deleted
    case Denied
}
class Reservation: PFObject, PFSubclassing {
    
    class func parseClassName() -> String {
        return "Reservation";
    }
    
    @NSManaged var resource : Resource!;
    @NSManaged var user : AUser!;
    @NSManaged var date : NSDate!;
    @NSManaged var halfHourPeriods : NSNumber!;
    @NSManaged var didAccess : NSNumber!
    
    class func dateFrom(week:Int, day:Int, hour:Int) -> NSDate
    {
        let comp = NSCalendar.currentCalendar().components([NSCalendarUnit.Year, NSCalendarUnit.Weekday, NSCalendarUnit.Month, NSCalendarUnit.Day, NSCalendarUnit.Hour], fromDate: NSDate())
        
        comp.year = 2015  //TODO: Needs to be fixed but good enough for demo on wednesday
        comp.weekday = 1
        comp.day = day + comp.day - 1 + ((week % 2) * 7)
        comp.hour = hour - 6
        
        let reservationDate = NSCalendar.currentCalendar().dateFromComponents(comp)
        return reservationDate!
    }
    
    class func makeReservation(theResource: Int, userID:Int, week:Int, day:Int, hour:Int, complete:(reservationCreated: ReservationAction, error : NSError!)-> Void)
    {
        let resourceQuery = PFQuery(className: "Resource");
        resourceQuery.whereKey("resourceID", equalTo : theResource);
        resourceQuery.getFirstObjectInBackgroundWithBlock { (resource, error) -> Void in
            let userQuery = PFQuery(className: "AUser")
            userQuery.whereKey("user", equalTo: PFUser.currentUser()!)
            userQuery.getFirstObjectInBackgroundWithBlock({ (user, error) -> Void in
                let resQuery = PFQuery(className: "Reservation")
                resQuery.whereKey("date", equalTo: dateFrom(week, day:day, hour:hour))
                resQuery.whereKey("resource", equalTo: resource!)
                resQuery.includeKey("user.employeeOf")
                resQuery.getFirstObjectInBackgroundWithBlock { (reservation, error) -> Void in
                    if error != nil
                    {
                        let reservation = Reservation();
                        reservation.user = user as! AUser
                        reservation.resource = resource as! Resource
                        reservation.date = dateFrom(week, day: day, hour: hour)
                        
                        reservation.saveInBackgroundWithBlock({ (success, error) -> Void in
                            complete(reservationCreated: .Created, error: nil)
                        })
                    }
                    else
                    {
                        let theReservation = reservation as! Reservation
                        print (theReservation.user.employeeOf.uniqueID)
                        if theReservation.user.employeeOf.uniqueID == theResource
                        {
                            theReservation.deleteInBackground()
                            complete(reservationCreated: .Deleted, error: nil)
                        }
                        else
                        {
                            complete(reservationCreated: .Denied, error: nil)
                        }
                    }
                }
                
            })
        }
    }
}
