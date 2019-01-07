### Snippet 1 - The function calc_spectra had previously contained about a hundred lines 
### of code with no useful comments. The output seemed to contain a bias, but
### the no one had been able to understand the function well enough to isolate
### the problem. I solved both issues by writing the function in a simpler 
### form. I created the function calc_normalization_params as a helper function
### for readability.
def calc_normalization_params(freqs, transform, t, height, mean_x, sdev_x, num_data_points):
    """ Calculate the normalized Fourier transform and frequency for the 
    Fourier transform of x
    Warning: A previous code version normalized segments, while this version 
    normalizes the entire data set at once. The previous version also included 
    a smoothing algorithm, which has been omitted for simplicity.
    @parameter: freqs, type = list or np.array
    @parameter: transform, type = list or np.array - this is the non-normalized Fourier transform
    @parameter: t, type = float - this is time
    @parameter: height, type = float - z in the Timeseries object
    @parameter: mean_x, type = float - the mean of the parameter of the Fourier transform F(x)
    @parameter: sdev_x, type = float - the standard deviation of x
    @parameter: num_data_points, type = int - the number of elements in x before the transform was found"""   

    ## DISCRETE SPECTRA
    transform = transform/num_data_points


    E = transform ** 2
    S = E * len(t)*(t[1]-t[0])
   
    ##  REDUCED FREQUENCY (PLOT and reference spectra)
    reduced_freq = np.abs(freqs*height/mean_x)
    reduced_transform = np.abs(np.meshgrid(S[0], freqs)[0]*S/sdev_x**2)

    ##  ALIASING
    aliasing = reduced_freq.size-9+np.hstack((np.where(
                                    	 np.diff(reduced_transform[-10:])>=0.)[0],[9]))[0]
    
    return reduced_transform, reduced_freq, aliasing

def calc_spectra(u_comp,v_comp,t_eq,height):
    """ Calculate dimensionless energy density spectra from an equidistant 
    time series.
    @parameter: u_comp, type = np.array or list
    @parameter: v_comp, type = np.array or list
    @parameter: t_eq, type = np.array or list """
    ## FREQUENCY
    freq = np.fft.fftfreq(np.size(u_comp),t_eq[1]-t_eq[0])

    ## FFT
    fft_u = np.fft.fft(u_comp)*1./np.size(u_comp)        #  normalized fft
    fft_v = np.fft.fft(v_comp)*1./np.size(v_comp) 
    uv_param = np.sqrt(fft_u * fft_v) # This is used in place of a true Fourier 
                                      # transform to calculate diagonal energy
    
    u_normalization_params = calc_normalization_params(freq, fft_u, t_eq, 
                                                       height, np.nanmean(u_comp), 
                                                       np.std(u_comp), len(u_comp))
    v_normalization_params = calc_normalization_params(freq, fft_v, t_eq, 
                                                       height, np.nanmean(v_comp), 
                                                       np.std(v_comp), len(v_comp))
    uv_normalization_params = calc_normalization_params(freq, uv_param, t_eq, 
                                                       height, np.nanmean(np.sqrt(v_comp*u_comp)), 
                                                       np.std(np.sqrt(v_comp*u_comp)), len(v_comp))
    
    return u_normalization_params[0],u_normalization_params[1],\
        v_normalization_params[1],uv_normalization_params[1],\
        u_normalization_params[2],v_normalization_params[2],\
        uv_normalization_params[2]
        
    #return f_sm,S_uu_sm,S_vv_sm,S_uv_sm,u_aliasing,v_aliasing,uv_aliasing
    
    
### Snippet 2 - My boss gave me significant freedom in deciding what new 
### functions to write for turbulence analysis, hoping that I would devise 
### an applicable and innovative approach. After reading several papers,
### I decided that a joint time frequency analysis using Forier transforms 
### would be a good place to start.

def plot_JTFA_STFT(u1, v1, t_eq, height, second_comp = 'v', window_length = 3500, fixed_limits = (None, None), ymax = None):
    """ Plots the joint time frequency analysis using a short-time Fourier
    transform smoothed and raw for both wind components in one figure. Returns
    the figure. To change overlap, 
    @parameter: u1: array of u-component perturbations
    @parameter: v1: array of second-component perturbations
    @parameter: t_eq: as defined by Timeseries
    @parameter: height: z as defined by Timeseries
    @parameter: second_comp, type = string: the name of the second measured
                wind component
    @parameter: window_length, type = int: window length in ms"""
    
    #set the window size to 3500 ms - this seems to caputure the relevant 
    #frequency range
    sampling_period = t_eq[1] - t_eq[0]
    pointsPerSegment = window_length / (sampling_period)
        
    
    # Analyze u - f is frequency, t is time, and Zxx is the Fourier transform
    f, t, Zxx = signal.stft(u1, fs = 1.0 / (sampling_period), \
    window = 'parzen', padded = False, noverlap = (pointsPerSegment/2), nperseg = pointsPerSegment) 
    # get nondimensionalized forms of f and Zxx - these are f*z/h and f*S/sigma^2, respectively
    reduced_transform_u1, reduced_freqs_u1, aliasing_u1 = wt.calc_normalization_params(f, Zxx, t, height, np.nanmean(u1), u1.std(dtype=float), len(t_eq))
   
    # Analyze second component
    f, t, Zxx = signal.stft(v1, fs = 1.0 / (sampling_period), \
    window = 'parzen', padded = False, noverlap = (pointsPerSegment/2), nperseg = pointsPerSegment)
    # and nondimensionalize    
    reduced_transform_v1, reduced_freqs_v1, aliasing_v1 = wt.calc_normalization_params(f, Zxx, t, height, np.nanmean(u1), u1.std(dtype=float), len(t_eq))
    reduced_transform_u1 *= 10e17
    reduced_transform_v1 *= 10e17
    # Create figure - 2x2 subplots
    fig, axarr = plt.subplots(2, 2)
    
    if(fixed_limits[0] is None or fixed_limits[1] is None):
        levels_u1 = np.linspace(np.nanmin(reduced_transform_u1.real), np.nanmax(reduced_transform_u1.real) * 0.05, 30)
    else:
        levels_u1 = np.linspace(fixed_limits[0], fixed_limits[1], 30)
        
    # Upper left plot - u1 
    axarr[0][0].set_yscale('log')
    im1 = axarr[0][0].pcolormesh(t, reduced_freqs_u1[f < 0.1], reduced_transform_u1.real[f < 0.1][:], cmap = 'winter', vmin = np.nanmin(reduced_transform_u1.real), vmax = np.nanmax(reduced_transform_u1.real) *0.05)
    axarr[0][0].set_xlabel('f*S/sigma^2')
    axarr[0][0].set_ylabel('Frequency (f*h/mean_u)')
    axarr[0][0].set_title('u\' STFT')
    if(not ymax is None):
        axarr[0][0].set_ylim((0, ymax))
    else:
        axarr[0][0].set_ylim(np.nanmean(reduced_freqs_u1[f < 0.1])/75)
    
    # Lower left plot - u1 smoothed
    axarr[1][0].set_yscale('log')
    im2 = axarr[1][0].contourf(t, reduced_freqs_u1[f < 0.1], reduced_transform_u1.real[f < 0.1][:], levels_u1, cmap = 'winter')
    axarr[1][0].set_xlabel('f*S/sigma^2')
    axarr[1][0].set_ylabel('Frequency (f*h/mean_u)')
    axarr[1][0].set_title('u\' STFT smoothed')
    if(not ymax is None):
        axarr[1][0].set_ylim((0, ymax))
    else:
        axarr[1][0].set_ylim(np.nanmean(reduced_freqs_u1[f < 0.1])/75)
   
    # Upper right plot - second comp
    axarr[0][1].set_yscale('log')
    im3 = axarr[0][1].pcolormesh(t, reduced_freqs_v1[f < 0.1], reduced_transform_v1.real[f < 0.1][:], cmap = 'winter', vmin = np.nanmin(reduced_transform_v1.real), vmax = np.nanmax(reduced_transform_v1.real) * 0.05)
    axarr[0][1].set_xlabel('f*S/sigma^2')
    axarr[0][1].set_ylabel('Frequency (f*h/mean_v)')
    axarr[0][1].set_title(second_comp + '\' STFT')
    if(not ymax is None):
        axarr[0][1].set_ylim((0, ymax))
    else:
        axarr[0][1].set_ylim(np.nanmean(reduced_freqs_v1[f < 0.1])/75)
    
    # Lower right plot - second comp smoothed
    axarr[1][1].set_yscale('log')
    im4 = axarr[1][1].contourf(t, reduced_freqs_v1[f < 0.1], reduced_transform_v1.real[f < 0.1][:], levels_u1, cmap = 'winter')
    axarr[1][1].set_xlabel('f*S/sigma^2')
    axarr[1][1].set_ylabel('Frequency (f*h/mean_v)')
    axarr[1][1].set_title(second_comp + '\' STFT smoothed')
    if(not ymax is None):
        axarr[1][1].set_ylim((0, ymax))
    else:
        axarr[1][1].set_ylim(np.nanmean(reduced_freqs_v1[f < 0.1])/75)


    print('reduced_transform u min ' + str(np.nanmin(reduced_transform_u1.real))
     + '\n                     max ' + str(np.nanmax(reduced_transform_u1.real))
     + '\n                     mean ' + str(np.nanmean(reduced_transform_u1.real)))
    print('reduced_freqs u     min ' + str(np.nanmin(reduced_freqs_u1.real))
     + '\n                     max ' + str(np.nanmax(reduced_freqs_u1.real))
     + '\n                     mean ' + str(np.nanmean(reduced_freqs_u1.real)))
    print('reduced_transform v min ' + str(np.nanmin(reduced_transform_v1.real))
     + '\n                     max ' + str(np.nanmax(reduced_transform_v1.real))
     + '\n                     mean ' + str(np.nanmean(reduced_transform_v1.real)))
    print('reduced_freqs v     min ' + str(np.nanmin(reduced_freqs_v1.real))
     + '\n                     max ' + str(np.nanmax(reduced_freqs_v1.real))
     + '\n                     mean ' + str(np.nanmean(reduced_freqs_v1.real)))
    cbar1 = fig.colorbar(im1, ax = axarr[0][0])
    cbar2 = fig.colorbar(im2, ax = axarr[1][0])
    cbar3 = fig.colorbar(im3, ax = axarr[0][1])
    cbar4 = fig.colorbar(im4, ax = axarr[1][1])

    if(fixed_limits != (None, None)):
        cbar1.set_clim(fixed_limits[0], fixed_limits[1])
        cbar2.set_clim(fixed_limits[0], fixed_limits[1])
        cbar3.set_clim(fixed_limits[0], fixed_limits[1])
        cbar4.set_clim(fixed_limits[0], fixed_limits[1])
    else:
        cbar1.set_clim(np.nanmin(reduced_transform_u1.real), np.nanmax(reduced_transform_u1.real)*0.05)
        cbar3.set_clim(np.nanmin(reduced_transform_v1.real), np.nanmax(reduced_transform_v1.real)*0.05)
     
    cbar1.update_normal(im1)
    cbar2.update_normal(im2)
    cbar3.update_normal(im3)
    cbar4.update_normal(im4)
        
    plt.tight_layout()
    
    return fig
    
### Snippet 3 - To identify directional turbulence, I decided to start with a 
### simple histogram approach. I experimented with a circular histogram,
### but others found it unintuitive. 

def wind_direction_mag_less_180(self):
        """ Return the wind direction in the range -180 to +180 degrees. The 
        'direction' list is in the range 0 to 360 degrees.
        """
        
        # Set up output array
        ret = np.zeros(len(self.direction))
        
        # Calculate equivalent angles for all directions greater than 180
        i = 0
        while (i < len(ret)):
            angle = self.direction[i]
            if(angle > 180):
                ret[i] = angle - 360
            else:
                ret[i] = angle
            i += 1
            
        return ret

def plot_wind_dir_hist(timeseriesObj):
    """ Simple wind direction histogram plot
    @parameter: Timeseries
    """

    wt.plots.plot_hist(timeseriesObj.wind_direction_mag_less_180())
    plt.title('Wind direction at ' + str(timeseriesObj.z) + 'm')
    plt.xlabel('Wind direction in degrees')
    plt.ylabel('Relative Frequency')

### Snippet 4 - I wanted to graphically represent the deviation of wind
### on a small time scale from the mean, but decided to use a local mean
### to better separate sensor noise from actual turbulence. I added the 
### variable tau to allow users to set the time scale of this local mean.

""" Timeseries is a class that holds data collected by the BSA software in
    the standard BSA software output. The class can hold die raw timeseries,
    the corresponding wtref, the components and coordinates of each
    measurement as well as the mean wind magnitude and the mean wind direction.
    The raw timeseries can be processed by nondimensionalising it, adapting the
    scale, making it equidistant and masking outliers. All the information in
    a Timeseries object can be saved to a txt file.
    @parameter: u, type = np.array
    @parameter: v, type = np.array
    @parameter: x, type = float
    @parameter: y, type = float
    @parameter: z, type = float
    @parameter: t_arr, type = np.array
    @parameter: t_transit, type = np.array
    @parameter: tau, type = int or float - time scale in milliseconds"""
    def __init__(self,u,v,x=None,y=None,z=None,t_arr=None,t_transit=None, tau = 10000):
        """ Initialise Timerseries() object. """
        self.x = x
        self.y = y
        self.z = z
        self.t_arr = t_arr
        self.t_transit = t_transit
        self.u = u
        self.u_unmasked = u
        self.v = v
        self.v_unmasked = v
        self.tau = tau # time scale in milliseconds
        self.weighted_u_mean = None
        self.weighted_comp_2_mean = None
        self.weighted_u_var = None
        self.weighted_comp_2_var = None
        self.scale = None
        self.wtref = None
        self.t_eq = None
        self.magnitude = None
        self.direction = None
        self.u1 = None
        self.v1 = None

    def set_tau(self, milliseconds):
        """ Give tau a new value """
        self.tau = milliseconds

    def calc_perturbations(self):
        """ Calculates u' and v' relative to the mean of each tau-long data 
        segment """
        start = 0
        i = 0
        j = 0
        
        self.u1 = []
        self.v1 = []
        
        while(i < len(self.t_eq)):
            if(self.t_eq[i] > self.t_eq[start] + self.tau):
                stop = i
                break
            i += 1
        
        while(True):
            
            # find index 'stop' of end of time segment
            while(i < len(self.t_eq)):
                if(self.t_eq[i] > self.t_eq[start] + self.tau):
                    stop = i
                    break
                i += 1
            
            # isolate the segment to be worked with. If the time measured is
            # not evenly divisible by tau, the last partial segment will be
            # discarded.
            try:
                u_seg = self.u[start : stop]
                v_seg = self.v[start : stop]
            except Exception:
                return
            
            # find the segment mean of each component
            u_mean = np.mean(u_seg)
            v_mean = np.mean(v_seg)
            
            while (j < len(self.t_eq) and self.t_eq[i] < stop):
                self.u1.append(self.u[j] - u_mean)
                self.v1.append(self.v[j] - v_mean)
                j += 1

### Snippet 5 - I created this function to graphically represent the 
### turbulence. A non-turbulent environment has a very distinct peak
### in the center of the graph, while a turbulent environment has a much 
### shorter peak and wider spread.

def plot_stdevs(u_unmasked, t_eq, tau, comp = 'u'):
    ''' This method plots the spread of an array based on how many standard 
    deviations each point is from the mean over each tau-long time period
    @parameter: the array to be analysed
    @parameter: the times corresponding to the array to be analysied (ms)
    @parameter: the characteristic time scale (ms) '''
    
    start = 0
    i = 0
    j = 0
    stdevs_from_mean = np.zeros(len(t_eq))
    
    # Get current axis
    ax = plt.gca()
       
    while(True):
        # find index 'stop' of end of time segment
        i = start
        if(start > len(t_eq)):
            h, bins = np.histogram(stdevs_from_mean, bins=6)
            h = h * 100. / len(u_unmasked)
            return ax.bar(bins[:-1], h)
        while(i < len(t_eq)):
            
            if(t_eq[i] > t_eq[start] + tau):
                
                stop = i
                break
            i += 1
    
        # isolate the segment to be worked with. If the time measured is
        # not evenly divisible by tau, the last partial segment will be
        # discarded.
        try:
            u_seg = u_unmasked[start : stop]
        except Exception:
            h, bins = np.histogram(stdevs_from_mean, bins=6)
            h = h * 100. / len(u_unmasked)
            return ax.bar(bins[:-1], h)

        # find the segment standard deviation
        stdev_u = np.std(u_seg)
        # find the segment mean
        u_mean = np.mean(u_seg)
        
        while(j < len(t_eq) and t_eq[i] < stop):
            if(np.abs(u_unmasked[j] - u_mean) > 1 * stdev_u):
                stdevs_from_mean[j] += 1
            if(np.abs(u_unmasked[j] - u_mean) > 2 * stdev_u):
                stdevs_from_mean[j] += 1
            if(np.abs(u_unmasked[j] - u_mean) > 3 * stdev_u):
                stdevs_from_mean[j] += 1
            if(np.abs(u_unmasked[j] - u_mean) > 4 * stdev_u):
                stdevs_from_mean[j] += 1
            if(np.abs(u_unmasked[j] - u_mean) > 5 * stdev_u):
                stdevs_from_mean[j] += 1
            if(np.abs(u_unmasked[j] - u_mean) > 6 * stdev_u):
                stdevs_from_mean[j] += 1
            j += 1
        
        start = stop
        stop = stop + tau



### Snippet 6 - The program was already able to plot a wind rose when I joined 
### the team, but the user could not adjust the resolution and the last sector
### often overlapped the first. My version of the function gave the user more
### control and spaced the sectors evenly.

def plotwindrose(inFF,inDD, num_bars = 10, ax = None, left_legend = False):
    """ Plots windrose with dynamic velocity classes of each 10% percentile and
    10 degree classes for directional data. The representation of the windrose 
    in this function is more detailed than in plot_DWD_windrose().
    @parameter inFF: np.array
    @parameter inDD: np.array
    @parameter num_bars: how many segments the degree range should be broken into
    @parameter ax: pyplot axes object, must be polar
    @left_legend: if true, the legend is positioned to the left of the plot instead of the right"""

    ffs = np.array([])
    percs = np.arange(0,100,10)
    for perc in percs:
        ffs = np.append(ffs,np.percentile(inFF,perc))
        
    factors_of_360 = np.array([1., 2., 3., 4., 5., 6., 8., 10., 12., 18., 20., 36., 40., 120., 360.])
    dd_range = min(factors_of_360[factors_of_360 > 360/num_bars]) # find appropriate degree width for each bar
    labels = []
    for i,f in enumerate(ffs[:-2]):
       labels.append(r'$'+'{0:.2f}-{1:.2f}'.format(f,ffs[i+1])+'\ ms^{-1}$')
    labels.append(r'$'+'>{0:.2f}'.format(ffs[-2])+'\ ms^{-1}$')
    
    ##  DATA PROCESSING
    dd,ff = Windrose(inDD,inFF).pack(dd_range,ffs)
    dd = dd*np.pi/180.
    
    ##  PLOT
    width = dd_range*np.pi/180.
    cmap = plt.cm.jet
    
    if(ax is None):
        ax = plt.subplot(111,polar=True)
        
    ax.bar(dd, ff[:,0],
           width=width,
           bottom=0.,
           facecolor=cmap(0),
           label=labels[0],
           align='edge',
           edgecolor='none')
    
    for i in range(ff[0].size-1):
       ax.bar(dd, ff[:,i+1],
              width=width,
              bottom=ff[:,i],
              facecolor=cmap(np.linspace(0, 1, ff[0].size)[i+1]),
              label=labels[i+1],
              align='edge',
              edgecolor='none')
       ff[:,i+1]=ff[:,i+1]+ff[:,i]
    ax.set_yticklabels([])
    ax.set_theta_zero_location("N")
    ax.set_theta_direction(-1)
    
    if(left_legend):
        bbox = (-1.15, 0.5)
    else:
        bbox = (1.25, 0.5)
    ax.legend(bbox_to_anchor=bbox, loc='center left',
                     borderaxespad=0.,fontsize=8, handlelength=1)
    # copied from https://stackoverflow.com/questions/9651092/my-matplotlib-pyplot-legend-is-being-cut-off/14246266
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.75, box.height])


### Snippet 7 - This was a function which I was very excited to create, but
### which was ultimately not included in the project. It utilized the 
### function in Snippet 6 to create a wind rose next to a similar diagram
### with perturbations instead of true wind direction. While the resulting
### plot was elegant, it was confusing and sometimes misleading. It taught 
### me to always seek feedback on the products I produce and to be prepared
### to change or eliminate them based on that feedback.

def plot_perturbation_rose(u1, v1, total_mag, total_direction, bar_divider = 3000, second_comp = 'v'):
    """ Plots a detailed wind rose using only the perturbation component of
    the wind. Number of bars depends on bar_divider and length of u1.
    @parameter: u1: array of u-component perturbations
    @parameter: v1: array of second-component perturbations
    @parameter: total_mag: array containing magnitude of wind (not perturbation)
    @parameter: total_direction: array containing direction of wind (not perturbation)
    @parameter: bar_divider: inversely proportional to number of bars to be plotted
    @parameter: second_comp, type = string: the name of the second measured
                wind component"""
                
    fig, axarr = plt.subplots(1, 2, subplot_kw=dict(projection='polar'))
    # Calculate perturbation direction
    unit_WD = np.arctan2(v1,u1) * 180/np.pi
    directions = (360 + unit_WD) % 360
    
    # Calculate perturbation magnitude
    speeds = np.sqrt(np.power(u1, 2) + np.power(v1, 2))
    
    # Plot the wind rose. Method called can be found in tools.py
    wt.plots.plotwindrose(total_mag, total_direction, len(total_mag) / bar_divider, ax = axarr[0], left_legend = True)
    wt.plots.plotwindrose(speeds, directions, len(u1) / bar_divider, ax = axarr[1])

    fig.suptitle('u-' + second_comp + ' plane', y = 0.8, x = 0.55)
    axarr[0].set_title('Wind Rose', y = 1.2)
    axarr[1].set_title('Perturbations', y = 1.2)

    axarr[0].set_position([0.2, 0.125, 0.4, 0.4])
    axarr[1].set_position([0.6, 0.125, 0.4, 0.4])