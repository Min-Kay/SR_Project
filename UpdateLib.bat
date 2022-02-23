

//// xcopy /옵션		.복사할파일이 담겨있는 경로		.복사 할 경로 

xcopy /y .\Engine\Public\*.h					.\Reference\Inc\
xcopy /y .\Engine\bin\*.lib						.\Reference\bin\
xcopy /y .\Engine\bin\*.dll						.\Client\bin\
xcopy /y .\Engine\bin\*.dll						.\Tool\bin\
xcopy /y .\fmod\64\f*.dll						.\Client\bin\