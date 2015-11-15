//
//  AppDelegate.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 11/8/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

import UIKit

let kParseApplicationID = "V5y9CHmlEF9n3wxITNogwnzvhr6TuPPwrPlY34pM";
let kParseClientKey = "j4peDjpdtO9MyhawlzyxRDNQxfJV6fhwGu4dAnzS";

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate, ESTBeaconManagerDelegate {

    var window: UIWindow?

    let beaconManager = ESTBeaconManager();

    func application(application: UIApplication, didFinishLaunchingWithOptions launchOptions: [NSObject: AnyObject]?) -> Bool {
        
        self.beaconManager.delegate = self;
        self.beaconManager.requestAlwaysAuthorization();
        
        self.beaconManager.startMonitoringForRegion(CLBeaconRegion(proximityUUID: NSUUID(UUIDString: "B9407F30-F5F8-466E-AFF9-25556B57FE6D")!, major: 39063, minor: 43563, identifier: "getdibbs panel"))
        
        UIApplication.sharedApplication().registerUserNotificationSettings(
            UIUserNotificationSettings(forTypes: .Alert, categories: nil))
        
        Parse.setApplicationId(kParseApplicationID, clientKey: kParseClientKey);
        
        Reservation.registerSubclass();
        Resource.registerSubclass();
        AUser.registerSubclass();
        Certification.registerSubclass();
        Group.registerSubclass();
        WaitList.registerSubclass();
        Company.registerSubclass();

        return true
    }
    
    func beaconManager(manager: AnyObject, didDetermineState state: CLRegionState, forRegion region: CLBeaconRegion) {
        if state == .Inside
        {
            NSNotificationCenter.defaultCenter().postNotificationName(kNotificationWithinProximity, object: nil)
        }
        else
        {
            NSNotificationCenter.defaultCenter().postNotificationName(kNotificationOutsideProximity, object: nil)
        }
    }
    func beaconManager(manager: AnyObject, didEnterRegion region: CLBeaconRegion) {
        NSNotificationCenter.defaultCenter().postNotificationName(kNotificationWithinProximity, object: nil)
    }
    
    func beaconManager(manager: AnyObject, didExitRegion region: CLBeaconRegion) {
        NSNotificationCenter.defaultCenter().postNotificationName(kNotificationOutsideProximity, object: nil)
    }
    
    func applicationWillResignActive(application: UIApplication) {
        // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
        // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
    }

    func applicationDidEnterBackground(application: UIApplication) {
        // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
        // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    }

    func applicationWillEnterForeground(application: UIApplication) {
        // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    }

    func applicationDidBecomeActive(application: UIApplication) {
        // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    }

    func applicationWillTerminate(application: UIApplication) {
        // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    }


}

