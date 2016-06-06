#pragma rtGlobals=1
Menu "Macros"
	"Load CELL_MinHop_PER_50", LoadCELL_MinHop_PER_50()
End
Macro LoadCELL_MinHop_PER_50()
	GBLoadWave/T={4,4}/W=2 "./CELL_MinHop_PER_50_W_CELL_MinHop_PER_50.bin"
	Rename wave0 CELL_MinHop_PER_50_NumFlows;
	Rename wave1 CELL_MinHop_PER_50_Throughput;
End
