import pandas as pd
import json
import matplotlib.pyplot as plt

with open("../build/output.json", "r") as f:
    content = json.load(f)

    content["global_info"]["thread_index"] = "global"
    order = ("thread_index", "elapsed_cycles", "elapsed_sec")
    content["global_info"] = dict((k, content["global_info"][k]) for k in order)

    content["thread_info"].insert(0, content["global_info"])

    df = pd.DataFrame(content["thread_info"])
    fig, axes = plt.subplots(nrows=1, ncols=2, figsize=(20, 8))
    df.plot(kind="bar", ax=axes[0], x="thread_index", y="elapsed_sec")
    df.plot(kind="bar", ax=axes[1], x="thread_index", y="elapsed_cycles")
    # axes[1].get_yaxis().get_major_formatter().set_scientific(False)
    plt.show()
