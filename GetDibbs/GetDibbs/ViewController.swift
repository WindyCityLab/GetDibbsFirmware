//
//  ViewController.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 11/8/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

import UIKit
import CoreBluetooth

let kNotificationWithinProximity = "kNotificationWithinProximity"
let kNotificationOutsideProximity = "kNotificationOutsideProximity"
let login = "kevin@thewcl.com"
let password = "pearson#dewitt1"
//let deviceID = "350038001547343339383037"
let deviceID = "2e001f000547343138333038"
let commandFunction = "setCursor"
let clientPresentFunction = "client"
let kMatrixRefresh = "matrix"
let kReserveEvent = "reserve"

class ViewController: UIViewController, GetDibbsPanelDelegate, PFLogInViewControllerDelegate {

    var conferencePanel : SparkDevice?
    var loginVC : PFLogInViewController!
    var company : Company!

    @IBOutlet weak var rssiLabel: UILabel!
    @IBOutlet weak var proximityIndicator: UIView!
    
    func latestRSSIValue(value: Int) {
        rssiLabel.text = "\(value)"
    }
    
    func fenceUpdate(isCloseToPanel: Bool) {
        if isCloseToPanel
        {
            proximityIndicator.backgroundColor = UIColor.blueColor()
            AUser.companyUniqueID({ (values) -> Void in
                self.conferencePanel?.callFunction(clientPresentFunction, withArguments: values, completion: { (number, error) -> Void in
                    if let e = error
                    {
                        let ctrl = UIAlertController(title: "Error", message: e.localizedDescription, preferredStyle: UIAlertControllerStyle.Alert)
                        let ok = UIAlertAction(title: "OK", style: UIAlertActionStyle.Default, handler: nil)
                        ctrl.addAction(ok)
                        self.presentViewController(ctrl, animated: true, completion: nil)
                    }
                })
            })
        }
        else
        {
            proximityIndicator.backgroundColor = UIColor.whiteColor()
            self.conferencePanel?.callFunction(clientPresentFunction, withArguments: [0,0,0,0], completion: { (number, error) -> Void in
                ()
            })
        }
    }
    
    @IBAction func logoutButtonTapped(sender: UIBarButtonItem) {
        PFUser.logOut()
        displayLoginIfNecessary()
    }
    func displayLoginIfNecessary()
    {
        if PFUser.currentUser() == nil
        {
            loginVC = PFLogInViewController();
            
            loginVC.delegate = self;
            presentViewController(loginVC, animated: false, completion: nil);
        }
        else
        {
            AUser.companyUniqueID({ (value) -> Void in
                self.assocaiteToGetDibbsPanel()
                AUser.companyUniqueID({ (value) -> Void in
                    print ("Company ID: \(value)")
                })
            })
        }
    }
    func logInViewController(logInController: PFLogInViewController, didLogInUser user: PFUser) {
        loginVC.dismissViewControllerAnimated(true, completion: nil);
        self.assocaiteToGetDibbsPanel()
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        
        NSNotificationCenter.defaultCenter().addObserverForName(kNotificationWithinProximity, object: nil, queue: NSOperationQueue.mainQueue()) { (notification) -> Void in
            self.fenceUpdate(true);
        }
        
        NSNotificationCenter.defaultCenter().addObserverForName(kNotificationOutsideProximity, object: nil, queue: NSOperationQueue.mainQueue()) { (notification) -> Void in
            self.fenceUpdate(false);
        }
        
        SparkCloud.sharedInstance().loginWithUser(login, password: password) { (error) -> Void in
            if let e = error
            {
                print("login failed: \(e.localizedDescription)");
            }
            else
            {
                SparkCloud.sharedInstance().getDevice(deviceID, completion: { (device, error) -> Void in
                    if (error == nil)
                    {
                        self.conferencePanel = device!
                        print("attached to device!")
                    }
                    else
                    {
                        print("Error: \(error.localizedDescription)")
                    }
                })
            }
        }
        SparkCloud.sharedInstance().subscribeToMyDevicesEventsWithPrefix(nil) { (theEvent, error) -> Void in
            print("got an event with name: \(theEvent.event) and data: \(theEvent.data)")
            if (theEvent.event == kReserveEvent)
            {
                let result = theEvent.data as NSString
                let items = result.componentsSeparatedByString(",")
                Reservation.makeReservation(Int(items[0])!,userID:Int(items[1])!, week: Int(items[2])!, day: Int(items[3])!, hour: Int(items[4])!, complete: { (reservationCreated, error) -> Void in
                    switch reservationCreated
                    {
                        case .Created:
                            self.conferencePanel?.callFunction(kMatrixRefresh, withArguments: ["Created"], completion: { (number, error) -> Void in
                                ()
                            })
                        case .Deleted:
                            self.conferencePanel?.callFunction(kMatrixRefresh, withArguments: ["Deleted"], completion: { (number, error) -> Void in
                                ()
                            })
                        case .Denied:
                            self.conferencePanel?.callFunction(kMatrixRefresh, withArguments: ["Denied!"], completion: { (number, error) -> Void in
                                ()
                            })
                    }
                })
            }
        }
    }

    @IBAction func updateCursor(sender: UISwitch) {
        if sender.on
        {
            AUser.companyUniqueID({ (value) -> Void in
                self.conferencePanel?.callFunction(clientPresentFunction, withArguments: value, completion: { (number, error ) -> Void in
                })
            })
        }
        else
        {
            AUser.companyUniqueID({ (value) -> Void in
                self.conferencePanel?.callFunction(clientPresentFunction, withArguments: [0,0,0,0], completion: { (number, error ) -> Void in
                })
            })
        }
    }
    @IBAction func directionButtonTapped(sender: UIButton) {
        switch sender.tag
        {
        case 0:
            conferencePanel?.callFunction("setCursor", withArguments: ["up"], completion: { (number, error) -> Void in
            })
        case 1:
            conferencePanel?.callFunction("setCursor", withArguments: ["right"], completion: { (number, error) -> Void in
            })
        case 2:
            conferencePanel?.callFunction("setCursor", withArguments: ["down"], completion: { (number, error) -> Void in
            })
        case 3:
            conferencePanel?.callFunction("setCursor", withArguments: ["left"], completion: { (number, error) -> Void in
            })
        default:
            ()
        }
    }
    func assocaiteToGetDibbsPanel()
    {
        GetDibbsPanel.sharedInstance.delegate = self
        GetDibbsPanel.sharedInstance.begin()
    }
    override func viewDidAppear(animated: Bool) {
        super.viewDidAppear(animated)
        displayLoginIfNecessary()
    }

}

