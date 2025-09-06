#!/usr/bin/env python3

from datetime import datetime
import sys
import argparse
import os
from lib.hdb import Hdb
import pandas as pd

import requests
import aquarius_webportal as aq



def debug(msg,v=True):
    if v: print(msg)

def main(args):
    sess=requests.Session()
    sess.cookies.set('disclaimer', 'accepted')
    ibwc=aq.AquariusWebPortal("waterdata.ibwc.gov", session=sess)

    debug(ibwc.fetch_dataset(dset_name="Discharge.Best Available@08364000",date_range="7Day"))
    #debug(ibwc.fetch_dataset(dset_name="Discharge.Daily Rounded@08364000",date_range="7Day"))

    #debug(ibwc.fetch_locations())
    #debug(ibwc.fetch_datasets(sites="08071000",parameterCd="00060",
    #                      start="2023-10-01",end="2023-10-10"), True)
    #debug(ibwc.get_param(param_name="Discharge"))


if __name__ == '__main__':
    main(sys.argv[:])

