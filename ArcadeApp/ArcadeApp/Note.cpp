/*

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <random>

using namespace std;

// int
//Signed integer type(can be negative).
//Typically 32 bits(range: -2, 147, 483, 648 to 2, 147, 483, 647).
//Used for general integer math.

// size_t
//Unsigned integer type(can’t be negative).
//Used to represent sizes and indices of memory or containers.
//Typically 64 bits on 64 - bit systems.
//Returned by functions like sizeof() or vector::size().



// L and R values and R values reference

// Ownership Transfer and Invalid State after Move
//-------------------------------------------------
//
// When an object is passed to a function taking a T&& (rvalue reference),
// you are giving up ownership of its internal resources.
//
// After moving, the original object is still *valid* (it can be destroyed safely),
// but its internal data is typically *empty* or *unspecified*.
// You should NOT rely on its old values or use it as if it still holds data.
//
// Example:

class BigObject{
};

void Consume(std::vector<BigObject>&& myVecOfBigObjects)
{

}

// Why noexcept?
// --------------
// • noexcept tells the compiler that the function will NOT throw exceptions.
// • STL containers (like std::vector) only use move constructors if they are noexcept.
// • If the move constructor might throw, STL will fallback to the slower copy version.



class A
{
public:
	A() = default; // Generate the default implementation for this function or constructor
	// tell compiler to auto-generate it 
	// If you define any constructor or special function(like copy / move constructors), the compiler stops generating the others automatically.
	// = default lets you bring them back explicitly.
	// When you define any constructor, the compiler no longer makes the “no-argument” one automatically.
	// So Foo f; fails.

	A(int a) {}
	A(const A& a) = delete; //delete: makes the class non copiebale
	A& operator=(const A& a) = delete; // The = delete syntax explicitly disables a function or constructor.
};

int main()
{
	std::vector<BigObject> data;
	Consume(std::move(data));  // Moves ownership of data into Consume()

	//  After std::move, 'data' is still valid but its contents are unspecified.
	std::cout << "After move, data.size() = " << data.size() << "\n"; // Typically 0

	// Don't do this: using moved-from objects as if they still hold data is undefined behavior.
	// data.push_back(4); // May work, but logically meaningless here.


	// Key takeaways:
	//  After std::move(x), treat x as an empty shell.
	//  You can assign a new value to x later, but don’t use its previous data.
	//  The destructor will still be called safely when x goes out of scope.
	//
	// Think of std::move() as: “I’m done with this object — someone else can take it.”

	// A

	A a;

	vector<int> intVec = { 1, 2, 3, 4, 5, 6 };

	// decltype

	for (decltype(intVec.size()) i = 0; i < intVec.size(); i++) // Here, intVec.size() returns an unsigned integer type, specifically std::vector<int>::size_type (usually size_t). so size_t i = 0;
	{
		cout << i << endl;
	}

	// Lambda

	vector<int> values = {10, 20, 3, 33};

	std::sort(values.begin(), values.end(), [](int v1, int v2) 
	{

	});

	int y = 5;
	int x = 3;


	auto lambdaFunc = [y](int x) // y is being capturedby copy or by reference &y if we put = instead of y it will capture all external variables by value or just & which captures everything by reference
	{
			std::cout << "x: " << x << " y: " << y << std::endl;
	};

	lambdaFunc(10);

	//DoSomthing([y, x]()
	//{
	//		cout << x << " " << y << endl;
	//});

	// random

	srand((unsigned int)time(NULL)); // time(NULL) returns the current time in seconds since 1970
	// srand sets a hidden variable that rand later uses

	int randomNum = rand() % 10;

	std::random_device r; // it gives a random number from the hardware You use it to initialize the generator
	std::default_random_engine el(r()); // it takes a seed from r and generates a sequence of numbers. passing the same seed will give the same sequence
	std::uniform_int_distribution<int> uniform_dist(1, 10); // defines the range

	int randomNumber = uniform_dist(el);

	cout << "Random number new: " << randomNumber << endl;
}

// lambda

void DoSomthing(std::function<void()> doSomthingElse)
{
	cout << "DOING SOMTHING" << endl;
	doSomthingElse();
}

*/