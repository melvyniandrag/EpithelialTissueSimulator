void MakeHistogram(string histName, vector<cell> sim_cells)
{
    // Some necessary things.
	ofstream histFile;
	int s;
    map<int, int>::iterator hist_it;
    
    //Make the histogram.
    map<int, int> histogram;
	histogram.clear();
	histogram.insert(pair<int, int>(3, 0));	
	histogram.insert(pair<int, int>(4, 0));
	histogram.insert(pair<int, int>(5, 0));
	histogram.insert(pair<int, int>(6, 0));
	histogram.insert(pair<int, int>(7, 0));
	histogram.insert(pair<int, int>(8, 0));
	histogram.insert(pair<int, int>(9, 0));
	histogram.insert(pair<int, int>(10, 0));
	histogram.insert(pair<int, int>(11, 0));
	for(auto c : sim_cells) 
    {
		s = c.GetNumSides();
		if ( histogram.find(s) != histogram.end())
		{
			hist_it = histogram.find(s);
			hist_it->second++;
		}
		else
		{
			histogram.insert(pair<int, int>(s, 0));
			hist_it = histogram.find(s);
			hist_it->second++;
		}
	}
	histFile.open(histName);
	for(hist_it=histogram.begin(); hist_it!= histogram.end(); hist_it++)
		histFile << hist_it->first << " " << hist_it->second << endl;
	histFile.close();
}

void MakePlots(vector<cell> sim_cells, double * X, double * Y)
{
    ofstream areaFile;
    areaFile.open("area.txt");
    ofstream perimFile;
    perimFile.open("perim.txt");
    for (auto c: sim_cells)
    {
    areaFile << c.ComputeArea(X, Y) << endl;
    perimFile << c.ComputePerimeter(X, Y) << endl;
    }
    areaFile.close();
    cout << "Plotting ..." << endl;
    system("python plot.py");
}
