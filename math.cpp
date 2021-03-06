#include "math.h"
#include <iostream>
/******************************************************************************/
/* randn()
 *
 * Normally (Gaussian) distributed random numbers, using the Box-Muller
 * transformation.  This transformation takes two uniformly distributed deviates
 * within the unit circle, and transforms them into two independently
 * distributed normal deviates.  Utilizes the internal rand() function; this can
 * easily be changed to use a better and faster RNG.
 *
 * The parameters passed to the function are the mean and standard deviation of
 * the desired distribution.  The default values used, when no arguments are
 * passed, are 0 and 1 - the standard normal distribution.
 *
 *
 * Two functions are provided:
 *
 * The first uses the so-called polar version of the B-M transformation, using
 * multiple calls to a uniform RNG to ensure the initial deviates are within the
 * unit circle.  This avoids making any costly trigonometric function calls.
 *
 * The second makes only a single set of calls to the RNG, and calculates a
 * position within the unit circle with two trigonometric function calls.
 *
 * The polar version is generally superior in terms of speed; however, on some
 * systems, the optimization of the math libraries may result in better
 * performance of the second.  Try it out on the target system to see which
 * works best for you.  On my test machine (Athlon 3800+), the non-trig version
 * runs at about 3x10^6 calls/s; while the trig version runs at about
 * 1.8x10^6 calls/s (-O2 optimization).
 *
 *
 * Example calls:
 * randn_notrig();	//returns normal deviate with mean=0.0, std. deviation=1.0
 * randn_notrig(5.2,3.0);	//returns deviate with mean=5.2, std. deviation=3.0
 *
 *
 * Dependencies - requires <cmath> for the sqrt(), sin(), and cos() calls, and a
 * #defined value for PI.
 */

/******************************************************************************/
//	"Polar" version without trigonometric calls
double randn_notrig(double mu, double sigma) {
    static bool deviateAvailable=false;	//	flag
    static float storedDeviate;			//	deviate from previous calculation
    double polar, rsquared, var1, var2;

    //	If no deviate has been stored, the polar Box-Muller transformation is
    //	performed, producing two independent normally-distributed random
    //	deviates.  One is stored for the next round, and one is returned.
    if(!deviateAvailable) {

        //	choose pairs of uniformly distributed deviates, discarding those
        //	that don't fall within the unit circle
        do {
            var1=2.0*(double(rand())/double(RAND_MAX)) - 1.0;
            var2=2.0*(double(rand())/double(RAND_MAX)) - 1.0;
            rsquared=var1*var1+var2*var2;
        } while(rsquared>=1.0 || rsquared == 0.0);

        //	calculate polar tranformation for each deviate
        polar=sqrt(-2.0*log(rsquared)/rsquared);

        //	store first deviate and set flag
        storedDeviate=var1*polar;
        deviateAvailable=true;

        //	return second deviate
        return var2*polar*sigma + mu;
    }

    //	If a deviate is available from a previous call to this function, it is
    //	returned, and the flag is set to false.
    else {
        deviateAvailable=false;
        return storedDeviate*sigma + mu;
    }
}


/******************************************************************************/
//	Standard version with trigonometric calls


double randn_trig(double mu, double sigma) {
    static bool deviateAvailable=false;	//	flag
    static float storedDeviate;			//	deviate from previous calculation
    double dist, angle;

    //	If no deviate has been stored, the standard Box-Muller transformation is
    //	performed, producing two independent normally-distributed random
    //	deviates.  One is stored for the next round, and one is returned.
    if(!deviateAvailable) {

        //	choose a pair of uniformly distributed deviates, one for the
        //	distance and one for the angle, and perform transformations
        dist=sqrt(-2.0 * log(double(rand()) / double(RAND_MAX)));
        angle=2.0 * PI * (double(rand()) / double(RAND_MAX));

        //	calculate and store first deviate and set flag
        storedDeviate=dist*cos(angle);
        deviateAvailable=true;

        //	calcaulate return second deviate
        return dist * sin(angle) * sigma + mu;
    }

    //	If a deviate is available from a previous call to this function, it is
    //	returned, and the flag is set to false.
    else {
        deviateAvailable=false;
        return storedDeviate*sigma + mu;
    }
}


bool intersects(Vector2 A, Vector2 B, Vector2 C, Vector2 D, Vector2 &intersection) {
    Vector2 E = B - A;
    Vector2 F = D - C;
    Vector2 P(-F.y(), F.x());
    Vector2 P2(-E.y(), E.x());
    double h = (Vector2::dotProduct(C, P) - Vector2::dotProduct(A, P)) /
               (Vector2::dotProduct(E, P));




    double i = (Vector2::dotProduct(A, P2) - Vector2::dotProduct(C, P2)) / Vector2::dotProduct(F, P2);


    if(!(h >= 0 && h <= 1 && i >= 0 && i <= 1)) {
        return false;
    }

    intersection = (E * h) + A;



    return true;
}

bool intersectsRect(Vector2 A, Vector2 B, double x, double y, double width, double height) {
    Vector2 C0(x, y);
    Vector2 D0(x + width, y);

    Vector2 C1(x + width, y);
    Vector2 D1(x + width, y + height);

    Vector2 C2(x + width, y + height);
    Vector2 D2(x, y + height);

    Vector2 C3(x, y + height);
    Vector2 D3(x, y);

    bool I0, I1, I2, I3;

    Vector2 buf;

    I0 = intersects(A, B, C0, D0, buf);
    I1 = intersects(A, B, C1, D1, buf);
    I2 = intersects(A, B, C2, D2, buf);
    I3 = intersects(A, B, C3, D3, buf);
    if(I0 || I1 || I2 || I3) {
        return false;
    }
    return true;
}

bool contains(double x0, double y0, double x1, double y1, double test_x, double test_y) {
    double slope = (y0 - y1) / (x0 - x1);
    double left  = y0 - test_y;
    double right = slope * (x0 - test_x);
    return left == right;
}

double distance(Vector2 v0, Vector2 v1) {
    return sqrt(pow(v0.x() - v1.x(), 2) + pow(v0.y() - v1.y(), 2));
}

double distance(double x0, double y0, double x1, double y1) {
    return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
}

bool is_nan(double val) {
    return !(val == val);
}

double d_abs(double val) {
    if(val < 0) val *= -1.0;
    return val;
}

Vector2::Vector2() {

}

Vector2::Vector2(double A, double B) {
  this->A = A;
  this->B = B;
}

double Vector2::x() {
  return A;
}

double Vector2::y() {
  return B;
}

Vector2 Vector2::operator+(Vector2& other) {
  return Vector2(this->x() + other.x(), this->y() + other.y());
}

Vector2 Vector2::operator-(Vector2& other) {
  return Vector2(this->x() - other.x(), this->y() - other.y());
}

Vector2 Vector2::operator*(const double& scalar) {
  return Vector2(this->x() * scalar, this->y() * scalar);
}

Vector2 Vector2::operator/(const double& scalar) {
  return Vector2(this->x() / scalar, this->y() / scalar);
}

double Vector2::dotProduct(Vector2& V0, Vector2& V1) {
  return V0.x() * V1.x() + V0.y() * V1.y();
}

