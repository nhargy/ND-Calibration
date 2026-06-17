import pandas as pd
import uproot
import argparse

def read_root_file(
        fn,
        treename='Hits'
        ):

    with uproot.open(fn) as f:
        tree = f[f"{treename}"]
        branches = ['eventID',
                    'pdgID',
                    'isEntry',
                    'fX1', 
                    'fY1', 
                    'fZ1',
                    'fX2', 
                    'fY2', 
                    'fZ2', 
                    'fK1',
                    'fK2',
                    'fEdep'
                    ]
        
        data = tree.arrays(
            branches, 
            filter_name='nsteps', 
            library='np'
            )

    return data

parser = argparse.ArgumentParser(description="Process ROOT file")
parser.add_argument("--rootfile", type=str,  help="Rootfile")
parser.add_argument("--outfile", type=str, default='output.parquet', help="Output file")

args = parser.parse_args()

print(f"Processing rootfile ==> {args.rootfile}")
data = read_root_file(args.rootfile)
df = pd.DataFrame(data)

df.to_parquet(args.outfile)
print("Saved!")
