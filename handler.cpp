#include "handler.h"

Handler::Handler() : 
	next_(NULL)
{
}
Handler::Handler(Handler* next) :
	next_(next)
{
}

Handler::~Handler()
{
	if(next_){
		delete next_;
	}
}

Handler::HANDLER_STATUS_T Handler::handle(TwitEng* eng, 
	const std::string& cmd, std::istream& instr) const
{
	// check if this handler can process this command
	if(canHandle(cmd)){
		return process(eng, instr);
	}
	// otherwise delegate to the next handler or return
	else if(next_ != NULL){ 
		return next_->handle(eng, cmd, instr);
	}
	// This was the last handler, so no one was able to process this command
	else {
		return HANDLER_KILL; // Error
	}
}
