#include <napi.h>
#include <windows.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")

std::string GetPrivilege(const std::wstring& username) {
  LPUSER_INFO_1 userInfo = nullptr;
  NET_API_STATUS status = NetUserGetInfo(NULL, username.c_str(), 1, (LPBYTE*)&userInfo);

  if (status == NERR_Success && userInfo != nullptr) {
    std::string privilege;
    switch (userInfo->usri1_priv) {
      case USER_PRIV_ADMIN:
        privilege = "Администратор";
        break;
      case USER_PRIV_USER:
        privilege = "Пользователь";
        break;
      case USER_PRIV_GUEST:
        privilege = "Гость";
        break;
      default:
        privilege = "Неизвестно";
    }
    NetApiBufferFree(userInfo);
    return privilege;
  } else {
    return "";
  }
}

Napi::String CheckPrivilege(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return Napi::String::New(env, "");
  }

  std::string username = info[0].As<Napi::String>();
  std::wstring wUsername(username.begin(), username.end());

  std::string privilege = GetPrivilege(wUsername);

  return Napi::String::New(env, privilege);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("checkPrivilege", Napi::Function::New(env, CheckPrivilege));
  return exports;
}

NODE_API_MODULE(addon, Init)
