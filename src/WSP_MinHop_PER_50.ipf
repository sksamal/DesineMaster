#pragma rtGlobals=1
Menu "Macros"
	"Load WSP_MinHop_PER_50", LoadWSP_MinHop_PER_50()
End
Macro LoadWSP_MinHop_PER_50()
	GBLoadWave/T={4,4}/W=2 "./WSP_MinHop_PER_50_W_WSP_MinHop_PER_50.bin"
	Rename wave0 WSP_MinHop_PER_50_NumFlows;
	Rename wave1 WSP_MinHop_PER_50_Throughput;
End
