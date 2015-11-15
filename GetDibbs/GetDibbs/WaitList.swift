//
//  WaitList.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 9/12/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

class WaitList : PFObject, PFSubclassing
{
    static func parseClassName() -> String {
        return "WaitList";
    }
    
    @NSManaged var forReservation : Reservation!;
    
}