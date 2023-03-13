%
% res=loadIMetronicData(pathExercise, pathDat, files, strDic)
% 
% Extract variables associated to the Imetronic .dat & corresponding exercise (.xls). 
% Can proccess multiple files (Tested with Poly-Avoidance, double check otherwise!).
%
% Input:
% 	pathExercise=fullpath to the exercise used.
% 	pathDat=path of .dat files to analyze    
%     files=struct that contains the prefixes of the filenames to analyze
%     strDic=dictionary of labels to mark as events (in the exercise, column _TI, _TL)
% Output:
%     res=struct containing the analyzed for each file (first file output is res(1), etc...).
%
% Example call:
%
% >> outVar=loadIMetronicData('D:\Daniel\AADATA\Exercises', 'D:\Daniel\AADATA\electro\g15', {'397','398'}, {'_CSM','_CSP'});


function res=loadIMetronicData(pathExercise, pathDat, files, strDic)

%list of files to load
if isempty(files)
    %load all .dat files within a folder
    filAux=dir([pathDat,'\*.dat']);
else
    k=1;
    for i=1:length(files)
        strAux=[pathDat,'\*',files{i},'*.dat'];
        %display(strAux);
        aux=dir(strAux);    
        
        aux1 = [aux(:).datenum].';
        [aux1,aux1] = sort(aux1);
        %[aux1,aux1] = sort({aux.name});
        aux=aux(aux1); % Cell array of names in order by datenum. 

        if isempty(aux)
            error(['ERROR: Files not found (-> ',strAux,')']);
        end
        for j=1:length(aux)
            filAux(k)=aux(j);
            k=1+k;
        end
    end
end

%read files
clear res;
for i=1:length(filAux)
    display(['Loding IME data: ',filAux(i).name])
    clear strDicId;
    
    %display(filAux(i).name);
    strAux = fileread([pathDat,'\',filAux(i).name]);
    
    %excercise info
    strAux2=textscan(strAux,'%s', 1,'HeaderLines',1,'Delimiter',',');
    strAux2=char(strAux2{1});
    indAux=findstr(strAux2,'\');
    indAux=indAux(end);
    strAux3 = fileread([pathExercise,strAux2(indAux:end)]);
    aux=textscan(strAux3, '%s', 1, 'HeaderLines',4);
    aux=char(aux{1});
    
    %get the minFreezTime.
    ind=findstr(aux,'ms):');
    minFz=str2num(aux(ind+4:end));%minFz=2000;%
    
    aux=textscan(strAux3, '%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s', 'Delimiter', '\t', 'HeaderLines',10);
    %strDicId=nan*ones(1,length(strDic));
    aux1=char(aux{1});
    aux2=char(aux{2});
    for j=1:length(strDic)
        strDicId{j}=[];
        for k=1:size(aux2,1)
            %if ~isempty(findstr(aux2(k,:),strDic{j}))
            if ~isempty(findstr(aux2(k,:),[strDic{j},'']))
                strDicId{j}=[strDicId{j},str2num(aux1(k,:))];
            end
        end
    end  
    aux2=char(aux{3});
    for j=1:length(strDic)
        for k=1:size(aux2,1)
            if ~isempty(aux2(k,:))
                %if ~isempty(findstr(strDic{j},aux2(k,:)))
                %if ~isempty(findstr(aux2(k,:),strDic{j}))
                if ~isempty(findstr(aux2(k,:),[strDic{j},'']))
                    strDicId{j}=[strDicId{j},str2num(aux1(k,:))];
                end
            end
        end
    end
    
    %get dat content into a matrix
    datCell=textscan(strAux, '%d %d %d %d %d %d %d %d %d %d %d %d', 'Delimiter', '\t','HeaderLines',13);
    datMat=cell2mat(datCell);
    
    %common info
    res(i).date=filAux(i).date;
    res(i).name=filAux(i).name;
    res(i).exerPath=[pathExercise,strAux2(indAux:end)];
    res(i).strDicId=strDic;
    res(i).totalLength=double(datMat(end,1))/1000;
    
    %look for events defined in strDic
    for j=1:length(strDic)
        if length(strDicId{j})>0
            res(i).strDicEv{j}=[];
            for k=1:length(strDicId{j})
                res(i).strDicEv{j}=[res(i).strDicEv{j};double(datMat(find(datMat(:,2)==10 & datMat(:,8)==strDicId{j}(k)),1))./1000];
            end
            res(i).strDicEv{j}=sort(res(i).strDicEv{j});
        else
            res(i).strDicEv{j}=nan;
        end
    end

    %get transition times of excercise; 
    aux=double(datMat(find(datMat(:,2)==9),[1,3,10:11]));
    indAux=find(aux(:,2)==1);
    aux(indAux,3)=-1*aux(indAux,3);
    aux(:,1)=aux(:,1)/1000;

    %remove esto de la traj q es un poco raro y repite timestamps
    indAux=find(aux(:,2)==0);
    aux(indAux,:)=[];

%     aux(find(aux(:,4)==0),:)=[];    %this line remove strange shit that was not related to tracking
    res(i).trajectories=aux;
    
    %fz based on trajectory
    res(i).minFz=1;
    indAux=find(diff(aux(:,1))>=(res(i).minFz));
    res(i).fzStart=aux(indAux,1);
    res(i).fzStop=aux(indAux+1,1);
        
end