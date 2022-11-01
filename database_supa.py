import os
from tkinter.messagebox import NO
from requests import Session, session
from supabase import *

url: str = "https://rgzdbypxpwnltpoxpztn.supabase.co"
key: str = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InJnemRieXB4cHdubHRwb3hwenRuIiwicm9sZSI6ImFub24iLCJpYXQiOjE2NjcxMjE1MjMsImV4cCI6MTk4MjY5NzUyM30.JYzTUeFAG-87ijixQ6V1pszdElQwVi9GHEgj5bF123U"
supabase: Client = create_client(url, key)
user = None
user_db = None
uid = 0

#  use this github to make api call https://github.com/supabase-community/supabase-py


def login_s(email: str, password: str, user_name: str) -> bool:
    try:
        global user, uid
        user = supabase.auth.sign_in(email=email, password=password)
        uid = user.access_token
        return True
    except:
        return False

def register_s(email: str, password: str, user_name: str) -> bool:
    try:
        global user, uid
        user = supabase.auth.sign_up(email=email, password=password)
        return True
    except:
        return False


# Make a function, that will fetch the database userdtb and return its contents as a list.
def get_userdtb():
    global user_db
    user_db = supabase.table("userdtb").select("*").execute()


def login_supa():
    print("Welcome to the login/register page.")
    get_userdtb()
    t_usr = input("Username: ")

