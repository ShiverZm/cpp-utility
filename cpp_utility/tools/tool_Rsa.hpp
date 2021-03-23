﻿////////////////////////////////////////////////////////////////////////////////
//
// Class Name:  tool_Rsa
// Description: RSA工具类
// Class URI:   https://github.com/fawdlstty/NetToolbox
// Author:      Fawdlstty
// Author URI:  https://www.fawdlstty.com/
// License:     此文件单独授权 以MIT方式开源共享
// Last Update: Jan 05, 2019
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_RSA_HPP__
#define __TOOL_RSA_HPP__

#include <string>
#include <Windows.h>
#include <openssl/pem.h>



class tool_Rsa {
public:
	// 生成公钥私钥对
	static bool generate (int bits, std::string file_pubkey, std::string file_rsapubkey, std::string file_rsaprvkey) {
		bool bExist = false;
		std::string info = International::translate ("The following file already exists. Do you want to overwrite it?");
		if (faw::Directory::exist (file_pubkey)) {
			bExist = true;
			info += "\n";
			info += file_pubkey;
		}
		if (faw::Directory::exist (file_rsapubkey)) {
			bExist = true;
			info += "\n";
			info += file_rsapubkey;
		}
		if (faw::Directory::exist (file_rsaprvkey)) {
			bExist = true;
			info += "\n";
			info += file_rsaprvkey;
		}
		if (bExist) {
			if (IDOK != ::MessageBoxA (NULL, info.c_str (), International::translate ("Info").data (), MB_ICONQUESTION | MB_OKCANCEL))
				return false;
		}

		RSA *r = ::RSA_new ();
		BIGNUM *bn = ::BN_new ();
		::BN_set_word (bn, 65537);
		::RSA_generate_key_ex (r, 2048, bn, nullptr);
		bool bSuccess = true;
		//
		int a = 0, b = 0, c = 0;
		if (!file_pubkey.empty ()) {
			BIO *bio = ::BIO_new_file (file_pubkey.c_str (), "w+");
			bSuccess &= !!::PEM_write_bio_RSA_PUBKEY (bio, r);
			::BIO_free (bio);
		}
		if (!file_rsapubkey.empty ()) {
			BIO *bio = ::BIO_new_file (file_rsapubkey.c_str (), "w+");
			bSuccess &= !!::PEM_write_bio_RSAPublicKey (bio, r);
			::BIO_free (bio);
		}
		if (!file_rsaprvkey.empty ()) {
			BIO *bio = ::BIO_new_file (file_rsaprvkey.c_str (), "w+");
			bSuccess &= !!::PEM_write_bio_RSAPrivateKey (bio, r, nullptr, nullptr, 0, nullptr, nullptr);
			::BIO_free (bio);
		}
		::BN_free (bn);
		::RSA_free (r);
		return bSuccess;
	}
};

#endif //__TOOL_RSA_HPP__
