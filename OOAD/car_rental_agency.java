//https://codereview.stackexchange.com/questions/129814/a-car-rental-agency-guaranteeing-uniqueness


/*

Car rental Agency

*/

package carhireapp;

import java.util.*;

/*
 * Author: Andrew Cathcart, S130315904
 * Main rental agency class
 * Contains the companies fleet of cars that they rent, as well as methods to get the currently 
 * rented cars, get the number of available cars of a certain size, see what car a certain 
 * driving licence is renting, issue a car to an individual with a valid licence and also terminate a rental.
 */
public class RentalAgency {
    private static List<Vehicle> ListOfCars = new ArrayList<Vehicle>();
    private static Map<DrivingLicence, Vehicle> FLEET = new HashMap<DrivingLicence, Vehicle>();

    // When RentalAgency is created, populate the ListOfCars
    public RentalAgency() {
        populateList();
    }

    // A method to populate the map of vehicles with 20 small cars and 10 large
    // cars
    private void populateList() {
        for (int i = 0; i < 20; i++) {
            ListOfCars.add(new SmallCar());
        }

        for (int i = 0; i < 10; i++) {
            ListOfCars.add(new LargeCar());
        }
    }

    // Returns the entire List listOfCars
    public List<Vehicle> getListOfCars() {
        return ListOfCars;
    }

    // Returns the entire map FLEET
    public Map<DrivingLicence, Vehicle> getFleet() {
        return FLEET;
    }

    /*
     * True for small, false for large. For all objects in the list, if the
     * vehicle in the list is a SmallCar object and is not rented, add to the
     * counter
     */
    public int availableCars(Boolean isSmall) {
        int count = 0;
        for (Vehicle temp : ListOfCars) {
            if (temp.isSmall() == isSmall)
                if (!temp.isRented()) {
                    count++;
                } else if (!temp.isRented()) {
                    count++;
                }
        }
        return count;
    }

    // Returns a list of vehicle objects that are currently rented
    public List<Vehicle> getRentedCars() {
        List<Vehicle> rentedCars = new ArrayList<Vehicle>();
        for (Vehicle temp : ListOfCars) {
            if (temp.isRented()) {
                rentedCars.add(temp);
            }
        }
        return rentedCars;
    }

    // Returns the car matching a driving licence
    public Vehicle getCar(DrivingLicence licence) {
        if (FLEET.containsKey(licence)) {
            return FLEET.get(licence);
        } else
            return null;
    }

    public void issueCar(DrivingLicence licence, Boolean isSmall) {
        Calendar dob = Calendar.getInstance();
        dob.setTime(licence.getDriverDateOfBirth());
        Calendar today = Calendar.getInstance();
        int age = today.get(Calendar.YEAR) - dob.get(Calendar.YEAR);

        if (today.get(Calendar.MONTH) < dob.get(Calendar.MONTH)) {
            age--;
        } else if (today.get(Calendar.MONTH) == dob.get(Calendar.MONTH)
                && today.get(Calendar.DAY_OF_MONTH) < dob.get(Calendar.DAY_OF_MONTH)) {
            age--;
        }

        Calendar doi = Calendar.getInstance();
        doi.setTime(licence.getDateOfIssue());
        int yearsHeld = today.get(Calendar.YEAR) - doi.get(Calendar.YEAR);
        if (today.get(Calendar.MONTH) < doi.get(Calendar.MONTH)) {
            yearsHeld--;
        } else if (today.get(Calendar.MONTH) == doi.get(Calendar.MONTH)
                && today.get(Calendar.DAY_OF_MONTH) < doi.get(Calendar.DAY_OF_MONTH)) {
            yearsHeld--;
        }
        /*
         * Code to calculate the age of the person and also how many years
         * they've held their licence Credited to user Zds from
         * stackoverflow.com and Irene Loos from coderanch.com
         * http://www.coderanch.com/t/391834/java/java/calculate-age
         * http://stackoverflow.com/questions/1116123/how-do-i-calculate-
         * someones-age-in-java
         */

        boolean flag = false;
        // A simple flag to toggle depending on if we find an appropriate car to
        // issue
        if ((licence.isFull()) && (!licence.getCurrentlyRenting())) {
            // If the individual has a full licence and is not currently renting
            // a car
            for (Vehicle temp : ListOfCars) {
                // iterates through the list of Vehicles
                if (temp.isSmall() == isSmall) {
                    // checks if the user entered true or false for isSmall and
                    // finds cars in the list from this
                    if ((age >= 21) && (yearsHeld >= 1)) {
                        // checks their current age and how many years they've
                        // owned their licence against the requirements
                        if ((!temp.isRented()) && (temp.isFull())) {
                            // It then checks that the car in the list is not
                            // rented and has a full tank
                            temp.setIsRented(true);
                            licence.setCurrentlyRenting(true);
                            FLEET.put(licence, temp);
                            flag = false;
                            break;
                        } else if ((age >= 25) && (yearsHeld >= 5) && (!temp.isRented()) && (temp.isFull())) {
                            temp.setIsRented(true);
                            licence.setCurrentlyRenting(true);
                            FLEET.put(licence, temp);
                            flag = false;
                            break;
                        } else
                            flag = true;
                    } else
                        flag = true;
                } else
                    flag = true;
            }
        } else
            flag = true;
        if (flag) {
            System.out.println("An appropriate car could not be issued");
        }
    }

    // Removes key:value pairs from a map when given a licence object
    // Also sets DrivingLicence's currentlyRenting status to false and Vehicle's
    // isRented status to false
    // Returns the fuel required to fill the tank, else -1
    public int terminateRental(DrivingLicence licence) {
        if (FLEET.containsKey(licence)) {
            int fuelRequiredToFill = ((FLEET.get(licence).getFuelCapacity()) - (FLEET.get(licence).getCurrentFuel()));
            licence.setCurrentlyRenting(false);
            FLEET.get(licence).setIsRented(false);
            FLEET.remove(licence);
            return fuelRequiredToFill;
        }
        return -1;
    }
}




public interface Vehicle {

    public String getRegNum();

    public int getFuelCapacity();

    public int getCurrentFuel();

    public void isTankFull();

    public boolean isFull();

    public boolean isRented();

    public void setIsRented(Boolean bool);

    public int addFuel(int amount);

    public int drive(int distance);

    public boolean isSmall();

}


public abstract class AbstractVehicle implements Vehicle {
    private RegistrationNumber regNum;
    private int fuelCapacity;
    private int currentFuel;
    private boolean isFull;
    private boolean isRented;

    public AbstractVehicle() {
        RegistrationNumber regNumObj = RegistrationNumber.getInstance();
        regNum = regNumObj;
        isFull = true;
        setIsRented(false);
    }

    public String getRegNum() {
        return regNum.getStringRep();
    }

    public void setFuelCapacity(int capacity) {
        this.fuelCapacity = capacity;
    }

    public int getFuelCapacity() {
        return fuelCapacity;
    }

    public int getCurrentFuel() {
        return currentFuel;
    }

    public void setCurrentFuel(int amount) {
        currentFuel = amount;
        isTankFull();
    }

    public void isTankFull() {
        if ((currentFuel - fuelCapacity) >= 0) {
            isFull = true;
        } else
            isFull = false;
    }

    // Calls the isTankFull method and then returns isFull
    public boolean getIsFull() {
        isTankFull();
        return isFull;
    }

    public boolean isRented() {
        return isRented;
    }

    public void setIsRented(Boolean bool) {
        isRented = bool;
    }

    public int addFuel(int amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("You must add an amount greater than zero");
        }

        if (isFull || !isRented) {
            return 0;
        }
        // If the tank is full or the car is not rented return zero

        if ((currentFuel + amount) <= fuelCapacity) {
            currentFuel += amount;
            if (currentFuel == fuelCapacity) {
                isFull = true;
                return amount;
            } else
                return amount;
        }
        // If the current fuel plus the amount to add is less than or equal to
        // the fuel capacity, add the amount to the current fuel and if the
        // current fuel is equal to the fuel capacity then set the boolean
        // isFull to true and return the amount added

        if ((currentFuel + amount) > fuelCapacity) {
            int difference = (fuelCapacity - currentFuel);
            currentFuel = fuelCapacity;
            isFull = true;
            return difference;
        }
        // Covers the case where the amount added would cause the current fuel
        // to exceed the fuel capacity

        return -1;
    }
}


public class SmallCar extends AbstractVehicle {
private int smallFuelCapacity = 45;
        private boolean isSmall = true;

        // Calls the super constructor, sets the fields appropriately
        public SmallCar() {
            super();
            super.setFuelCapacity(smallFuelCapacity);
            super.setCurrentFuel(smallFuelCapacity);
        }

        public boolean isSmall() {
            return isSmall;
        }

        // returns the number of whole Litres of fuel consumed during the journey
        public int drive(int distance) {
            int fuelUsed = 0;

            if (distance < 0) {
                throw new IllegalArgumentException("Distance cannot be less than zero");
            }

            if (super.isRented() && (super.getCurrentFuel() > 0)) {
                fuelUsed = (distance / 25);
                super.setCurrentFuel(super.getCurrentFuel() - fuelUsed);
                return fuelUsed;
            }
            return fuelUsed;
        }


        public boolean isFull() {
            boolean bool = super.getIsFull();
            return bool;
        }
    }

public class LargeCar extends AbstractVehicle {
    private int largeFuelCapacity = 65;
    private boolean isSmall = false;

    // Calls the super constructor, sets the fields appropriately
    public LargeCar() {
        super();
        super.setFuelCapacity(largeFuelCapacity);
        super.setCurrentFuel(largeFuelCapacity);
    }

    public boolean isSmall() {
        return isSmall;
    }

    // returns the number of whole Litres of fuel consumed during the journey
    public int drive(int distance) {
        int fuelUsed = 0;

        if (distance < 0) {
            throw new IllegalArgumentException("Distance cannot be less than zero");
        }

        if (super.isRented() && (super.getCurrentFuel() > 0)) {
            if (distance <= 50) {
                fuelUsed = (distance / 15);
                super.setCurrentFuel(super.getCurrentFuel() - fuelUsed);
                return fuelUsed;
            } else {
                int moreThan = (distance - 50);
                fuelUsed = (50 / 15) + (moreThan / 20);
                super.setCurrentFuel(super.getCurrentFuel() - fuelUsed);
                return fuelUsed;
            }
        }

        return fuelUsed;
    }


    public boolean isFull() {
        boolean bool = super.getIsFull();
        return bool;
    }
}


import java.util.HashMap;
import java.util.Map;
import java.util.Random;

public final class RegistrationNumber {
    private static final Map<String, RegistrationNumber> REGNUM = new HashMap<String, RegistrationNumber>();
    // Stores stringRep with object
    private final char letter; // One letter
    private final int numbers; // Four numbers
    private final String stringRep; // letter + number, e.g. A1234

    private RegistrationNumber(char letter, int numbers) {
        this.letter = letter;
        this.numbers = numbers;
        this.stringRep = String.format("%s%04d", letter, numbers);
        // Pad the string to make sure we always get a four digit number
    }

    public static RegistrationNumber getInstance() {
        Random random = new Random();
        // Using the random class instead of math.random as it is a static
        // method
        final Character letter = (char) (random.nextInt(26) + 'A');
        final int numbers = random.nextInt(9000) + 1000;
        final String stringRep = letter + numbers + "";

        if (!REGNUM.containsKey(stringRep)) {
            REGNUM.put(stringRep, new RegistrationNumber(letter, numbers));
        }
        // If the randomly generated registration plate is unique then create a
        // new object and return a reference to it
        else if (REGNUM.containsKey(stringRep)) {
            return getInstance();
        }
        // If the randomly generated registration plate is not unique, call the
        // getInstance method again

        return REGNUM.get(stringRep);
        // return a reference
    }

    public char getLetter() {
        return letter;
    }

    public int getNumbers() {
        return numbers;
    }

    public String getStringRep() {
        return stringRep;
    }

    public String toString() {
        return "RegistrationNumber [letter=" + letter + ", numbers=" + numbers + ", stringRep=" + stringRep + "]";
    }
}


public final class LicenceNumber {
    private static final Map<String, LicenceNumber> LICENCENUM = new HashMap<String, LicenceNumber>();
    private final String initials;
    private final int yearOfIssue;
    private final int serialNum;
    private final String stringRep;

    private LicenceNumber(String initials, int yearOfIssue2, int serialNum) {
        this.initials = initials;
        this.yearOfIssue = yearOfIssue2;
        this.serialNum = serialNum;
        stringRep = initials + "-" + yearOfIssue2 + "-" + serialNum;
    }

    public static LicenceNumber getInstance(Name fullName, Date dateOfIssue) {
        final String initials = fullName.getFirstName().substring(0, 1) + fullName.getLastName().substring(0, 1);
        Calendar cal = Calendar.getInstance();
        cal.setTime(dateOfIssue);
        final int yearOfIssue = cal.get(Calendar.YEAR);
        Random r = new Random();
        // Using the random class instead of math.random as it is a static
        // method
        final int serialNum = r.nextInt(11);
        final String stringRep = initials + "-" + yearOfIssue + "-" + serialNum;

        if (!LICENCENUM.containsKey(stringRep)) {
            LICENCENUM.put(stringRep, new LicenceNumber(initials, yearOfIssue, serialNum));
        } else if (LICENCENUM.containsKey(stringRep)) {
            return getInstance(fullName, dateOfIssue);
        }
        // If two people have the same name, date of birth and are generated the
        // same serial number, call the getInstance again

        return LICENCENUM.get(stringRep);
        // If the licence number is unique then create a
        // new object, put it into the HashMap and return a reference to it,
        // else return a reference
    }

    public String getInitials() {
        return initials;
    }

    public int getYearOfIssue() {
        return yearOfIssue;
    }

    public int getSerialNum() {
        return serialNum;
    }

    public String getStringRep() {
        return stringRep;
    }

    @Override
    public String toString() {
        return "LicenceNumber [initials=" + initials + ", yearOfIssue=" + yearOfIssue + ", serialNum=" + serialNum
                + ", stringRep=" + stringRep + "]";
    }
}


public final class Name {

    private final String firstName;
    private final String lastName;

    public Name(String firstName, String lastName) {
        if ((firstName == null) || (firstName.isEmpty())) {
            throw new IllegalArgumentException("firstName cannot be null or empty");
        }
        if ((lastName == null) || (lastName.isEmpty())) {
            throw new IllegalArgumentException("lastName cannot be null or empty");
        }
        this.firstName = firstName;
        this.lastName = lastName;
    }

    public String getFirstName() {
        return firstName;
    }

    public String getLastName() {
        return lastName;
    }

    @Override
    public String toString() {
        return "firstName= " + firstName + " lastName= " + lastName;
    }
 }
 
 public final class DrivingLicence {
    private final Name driverName;
    private final Date driverDateOfBirth;
    private final Date dateOfIssue;
    private final LicenceNumber number;
    private final boolean isFull;
    private boolean currentlyRenting = false;

    public DrivingLicence(Name driverName, Date dateOfBirth, Date dateOfIssue, boolean isFull) {
        this.driverName = driverName;
        this.driverDateOfBirth = dateOfBirth;
        this.dateOfIssue = dateOfIssue;
        this.number = LicenceNumber.getInstance(driverName, dateOfIssue);
        this.isFull = isFull;
    }

    public Name getDriverName() {
        return driverName;
    }

    public Date getDriverDateOfBirth() {
        return driverDateOfBirth;
    }

    public Date getDateOfIssue() {
        return dateOfIssue;
    }

    public LicenceNumber getNumber() {
        return number;
    }

    public boolean isFull() {
        return isFull;
    }

    public void setCurrentlyRenting(Boolean bool) {
        currentlyRenting = bool;
    }

    public boolean getCurrentlyRenting() {
        return currentlyRenting;
    }

    @Override
    public String toString() {
        return "\nDrivingLicence \ndriverName= " + driverName.toString() + "\ndriverDateOfBirth= " + driverDateOfBirth
                + "\ndateOfIssue= " + dateOfIssue + "\nnumber= " + number.toString() + "\nisFull= " + isFull + "]";
    }

}


