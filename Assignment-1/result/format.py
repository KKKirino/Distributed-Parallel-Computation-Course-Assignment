"""用于从结果生成 Latex 表格"""

import os
from typing import List

def get_path(path: str):
  return os.path.join(os.path.dirname(os.path.abspath(__file__)), path)

def run():
  lines = []

  fpath = get_path('./big.txt')
  print(fpath)

  with open(fpath, 'r') as f:
    lines = f.readlines()

  lines: List[str] = [l.replace('\n', '') for l in lines][1:]
  data = []
  for i in range(0, len(lines), 13):
    group = lines[i:i + 13]
    p_count = int(group[0].split(': ')[1])
    group_data = {}
    for j in range(1, 13, 2):
      name = group[j].replace(': ', '')
      time = float(group[j + 1].split(' ')[-1])
      group_data[name] = time
    data.append(group_data)
  group_with_name = {}
  for group in data:
    for (k, v) in group.items():
      if k not in group_with_name: group_with_name[k] = []
      group_with_name[k].append(v)
  text = []
  for (k, v) in group_with_name.items():
    text.append('{} & {}'.format(k, ' & '.join(['{:.2f}'.format(x) for x in v])))
  text = '\\\\\n'.join(text)
  print(text)

if __name__ == '__main__':
  run()