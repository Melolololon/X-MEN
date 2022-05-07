#pragma once
#include<string>

namespace MelLib 
{
	// Stringに関係する便利関数を持ったクラス
	class StringSupport
	{
	public:
		/// <summary>
		/// 渡したパスからファイル名を抜き出します。
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		static std::string ExtractionFileName(const std::string& path);

	};
}

