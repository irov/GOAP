#	pragma once

#	include <exception>

namespace GOAP
{
	class Exception
		: public std::exception
	{
	public:
        Exception( const char * _message );
		~Exception() throw();

	public:
		const char * what() const throw() override;

	protected:
		const char * m_message;
	};

    namespace Helper
    {
        void throw_exception( const char * _format, ... );
    }
}

