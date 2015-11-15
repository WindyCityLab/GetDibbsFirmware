//
//  AUser.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 9/12/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

class AUser : PFObject, PFSubclassing
{
    class func parseClassName() -> String {
        return "AUser";
    }
    
    @NSManaged var user : PFUser!
    @NSManaged var points : NSNumber!
    @NSManaged var employeeOf : Company!
    @NSManaged var userID : String!
    
    class func companyUniqueID(complete:(value : [Int]) -> Void)
    {
        let query = PFQuery(className: "AUser");
        query.whereKey("user", equalTo: PFUser.currentUser()!);
        query.includeKey("employeeOf");
        query.getFirstObjectInBackgroundWithBlock({ (me, error) -> Void in
            if let c = (me as! AUser).employeeOf
            {
                var result : [Int] = Array()
                result.append(c.uniqueID.integerValue)
                result.append(c.red.integerValue)
                result.append(c.green.integerValue)
                result.append(c.blue.integerValue)
                complete (value:result);
            }
        })
    }
}