#!/bin/python3

from sys import argv
from time import sleep
from dataclasses import dataclass
@dataclass
class deer:
    name: str
    speed: int
    duration: int
    rest: int
    points: int = 0
    travelled: int = 0

    def cycle_time(self) -> int:
        return self.duration + self.rest
    
    def distance(self, t: int) -> int:
        total_cycles = t // self.cycle_time()
        remainder = t % self.cycle_time()
        remainder_travelling = min(remainder, self.duration)

        return ((self.duration*total_cycles) + remainder_travelling) * self.speed
def main() -> None:
    # TODO: Read input??
    deers: list[deer] = []
    dur = 2503
    with open(argv[1]) as fp:
        ln = (fp.readline())
        while ln:
            print(ln)
            ln = ln.split(' ')
            if ln: deers.append(deer(ln[0], int(ln[3]), int(ln[6]), int(ln[13])))
            ln = (fp.readline())
    for t in range(1, dur+1):
        lead = 0
        _max = 0
        for k in range(len(deers)):
            v = deers[k]
            dist = v.distance(t)
            if dist > _max:
                _max = dist
                lead = k
        deers[lead].points += 1
    pts = [
        d.points for d in deers
    ]
    print(max(pts))
main()
