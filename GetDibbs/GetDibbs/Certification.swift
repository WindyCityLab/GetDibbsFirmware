//
//  Certification.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 9/12/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

class Certification : PFObject, PFSubclassing
{
    static func parseClassName() -> String {
        return "Certification";
    }
    
    @NSManaged var theUser : AUser!
    @NSManaged var theResource : Resource!
    @NSManaged var dateOfCertificationGranted : NSDate!
    
}
