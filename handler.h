#ifndef HANDLER_H
#define HANDLER_H
#include <string>
#include <istream>
#include "twiteng.h"

class Handler 
{
public:
	/** 
	 * Return status options from process()
	 * 	 HANDLER_OK    = successful and processing should continue
	 *   HANDLER_QUIT  = successful and processing should stop / program should quit
	 *   HANDLER_ERROR = an error occurred but processing can continue
	 *   HANDLER_KILL  = an error occurred and the program should halt immediately
	 */
	enum HANDLER_STATUS_T {HANDLER_OK = 0, HANDLER_QUIT, HANDLER_ERROR, HANDLER_KILL};

	/**
	 *  Default constructor
	 */
	Handler();

	/**
	 * Initializing constructor
	 * @param [in] next 
	 *    Pointer to next constructor
	 */
	Handler(Handler* next);

	/**
	 * Destructor
	 */
	virtual ~Handler();

	/**
	 * Initializing constructor
	 *
	 * @param [in] end 
	 *   Twitter engine
	 * @param [in] cmd
	 *   Command name attempting to be handled
	 * @param [inout] instr
	 *   Input stream to read additional command arguments, if needed
	 * @return
         *   Appropriate HANLDER_STATUS_T value
	 */
	HANDLER_STATUS_T handle(TwitEng* eng, const std::string& cmd, 
		std::istream& instr) const;

protected:

	/**
	 * Determines if this handler can handle the command given by cmd
	 *
	 * @param[in] cmd
	 *   Command name attempting to be handled
	 * @return 
	 *   true, if this Handler can process cmd
	 *   false, otherwise
	 */

	virtual bool canHandle(const std::string& cmd) const = 0;

	/**
	 * Processes the command
	 *
	 * @param [in] end 
	 *   Twitter engine
	 * @param [inout] instr
	 *   Input stream to read additional command arguments, if needed
	 */
	virtual HANDLER_STATUS_T process(TwitEng* eng, std::istream& instr) const = 0;

private:
	Handler* next_;
};

#endif
