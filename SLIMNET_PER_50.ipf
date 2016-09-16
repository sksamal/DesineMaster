#pragma rtGlobals=1
Menu "Macros"
	"Load SLIMNET_PER_50", LoadSLIMNET_PER_50()
End
Macro LoadSLIMNET_PER_50()
	GBLoadWave/T={4,4}/W=2 "./SLIMNET_PER_50_W_SLIMNET_PER_50.bin"
	Rename wave0 SLIMNET_PER_50_NumFlows;
	Rename wave1 SLIMNET_PER_50_Throughput;
End
