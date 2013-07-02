#ifndef BADLEVELDESCRIPTION_H
#define BADLEVELDESCRIPTION_H

#include <stdexcept>
#include <string>

class BadLevelDescription : public std::exception
{
	public:
		explicit BadLevelDescription(const char bad_char) : bad_(bad_char) {}
		virtual const char* what() const throw();

	private:
		char bad_;
};

#endif // BADLEVELDESCRIPTION_H
