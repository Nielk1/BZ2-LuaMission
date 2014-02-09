#ifndef TRNAllies_H__
#define TRNAllies_H__

class TRNAllies
{
	public:

	// Pass in a trn file name, e.g. "bridges2i.trn", and it'll set up
	// default team allies specified in the trn file. See changelog
	// for exact specs of what needs to be in the TRN.
	static void SetupTRNAllies(const char* pTRNName);

};

#endif // #ifndef TRNAllies_H__
