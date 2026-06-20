import pandas as pd

v3_main    = pd.read_csv('../../raw_data_pipeline_v3/dataset/extracted_features_v3.csv')
v3_sandbox = pd.read_csv('../dataset_sandbox/extracted_features_v3_sandbox.csv')

print("=== PERBANDINGAN DATASET ===")
print(f"Main V3    : {len(v3_main)} windows, {v3_main['subject_id'].nunique()} subjek")
print(f"Sandbox V3 : {len(v3_sandbox)} windows, {v3_sandbox['subject_id'].nunique()} subjek")

print()
print(f"{'Subjek':<12} {'Main':>8} {'Sandbox':>10} {'Status':>8}")
print("-" * 42)

all_subj = sorted(set(v3_main['subject_id']) | set(v3_sandbox['subject_id']))
all_same = True
for s in all_subj:
    m  = v3_main[v3_main['subject_id'] == s]
    sb = v3_sandbox[v3_sandbox['subject_id'] == s]
    mn  = (m['bp_class'] == 0).sum();  mh  = (m['bp_class'] == 1).sum()
    sbn = (sb['bp_class'] == 0).sum(); sbh = (sb['bp_class'] == 1).sum()
    sama = "OK" if (mn == sbn and mh == sbh) else "BEDA"
    if sama == "BEDA":
        all_same = False
    print(f"{s:<12} {mn}N/{mh}H     {sbn}N/{sbh}H     {sama:>8}")

print()
if all_same:
    print("Distribusi per subjek: IDENTIK")
else:
    print("Distribusi per subjek: ADA PERBEDAAN")

print()
print("=== CEK RATA-RATA FITUR ===")
cols = ['ir_ratio', 'red_ratio', 'crest_time_ratio', 'heart_rate']
for c in cols:
    mv = v3_main[c].astype(float).mean()
    sv = v3_sandbox[c].astype(float).mean()
    diff = abs(mv - sv)
    pct  = (diff / mv) * 100
    status = "OK" if pct < 1.0 else "BEDA"
    print(f"  {c:<24}: Main={mv:.5f}  Sandbox={sv:.5f}  Selisih={pct:.2f}%  {status}")
